//****************************************************************************************
//
//	INCLUDE FILES
//
//****************************************************************************************
#include	<iostream>

#include	"GateControl.hpp"
#include	<map>
#include <vector>

using namespace std;

//****************************************************************************************
//
//	CONSTANT DEFINITIONS
//
//****************************************************************************************

//****************************************************************************************
//
//	CLASSES, TYPEDEFS AND STRUCTURES
//
//****************************************************************************************

//****************************************************************************************
//
//	PUBLIC DATA
//
//****************************************************************************************
extern	string	gCurrentDate;

extern	string	gCurrentTime;

//****************************************************************************************
//
//	PRIVATE DATA
//
//****************************************************************************************

//****************************************************************************************
//
//	FUNCTION PROTOTYPES
//
//****************************************************************************************

//****************************************************************************************
//	GateControl::AccessAllowed
//****************************************************************************************
bool	GateControl::AccessAllowed(CardNumber number)
{
    bool allowed = false;
    
    AuthorizationIterator it;
    
    it = authorizationMap_.find(number);
  
    // check if number is in map
    if (it == authorizationMap_.end()){
        //not found
        Transaction trans2(number, "***", gCurrentDate, gCurrentTime, false);
        transactionVector_.push_back(trans2);
        return allowed;
    } else {
        // check if times are valid
        if ((it->second.startTime_ <= gCurrentTime)&&(it->second.endTime_ >= gCurrentTime)){
            Transaction trans1(number, it->second.name_, gCurrentDate, gCurrentTime, true);
            transactionVector_.push_back(trans1);
            return allowed;
            
        //times are not valid, but still record in transaction log
        } else {
            allowed = false;
            Transaction trans1(number, it->second.name_, gCurrentDate, gCurrentTime, false);
            transactionVector_.push_back(trans1);
            return allowed;
        }
    }
}

//****************************************************************************************
//	GateControl::AddAuthorization
//****************************************************************************************
bool	GateControl::AddAuthorization(CardNumber number, const string& name,
									  const string& startTime, const string& endTime)
{
    bool authorized = true;
    
    Authorization auth1;
    auth1.number_ = number;
    auth1.name_ = name;
    auth1.startTime_ = startTime;
    auth1.endTime_ = endTime;
    
    AuthorizationIterator it;
    it = authorizationMap_.find(number);
    
    //if number is found, put the key and value in the map
    if (it == authorizationMap_.end()) {
        authorizationMap_[number] = auth1;
    } else {
        authorized = false;
    }
    
	return authorized;
}

//****************************************************************************************
//	GateControl::ChangeAuthorization
//****************************************************************************************
bool	GateControl::ChangeAuthorization(CardNumber number, const string& name,
										 const string& startTime, const string& endTime)
{
	bool authorized = false;
    
    Authorization auth1;
    
    auth1.number_ = number;
    auth1.name_ = name;
    auth1.startTime_ = startTime;
    auth1.endTime_ = endTime;
    
    AuthorizationIterator it;
    it = authorizationMap_.find(number);
    if (it == authorizationMap_.end()){
        //not found
        return authorized;
    } else {
        //erase specified authorization and replace it with new one
        authorizationMap_.erase(it);
        authorizationMap_[number] = auth1;
        authorized = true;
    }
    return authorized;
}

//****************************************************************************************
//	GateControl::DeleteAuthorization
//****************************************************************************************
bool	GateControl::DeleteAuthorization(CardNumber number)
{
    bool authorized = false;
    
    Authorization auth;
    AuthorizationIterator it;
    it = authorizationMap_.find(number);
    if (it == authorizationMap_.end()){
        //not found
        return authorized;
    } else {
        auth = authorizationMap_[number];
        //erase value at specified location
        authorizationMap_.erase(it);
        authorized = true;
    }
    
    return authorized;

}

//****************************************************************************************
//	GateControl::GetAllAuthorizations
//****************************************************************************************
void	GateControl::GetAllAuthorizations(AuthorizationVector& authorizationVector)
{
    for (int i = 0; i < authorizationVector.size(); i++) {
        authorizationVector.pop_back();
    }
    AuthorizationMap::iterator it;
    //push all authorizations into vector
    for (it = authorizationMap_.begin(); it != authorizationMap_.end(); it++) {
        authorizationVector.push_back(it -> second);
    }
}

//****************************************************************************************
//	GateControl::GetAllTransactions
//****************************************************************************************
void	GateControl::GetAllTransactions(TransactionVector& transactionVector)
{

    for (int i = 0; i < transactionVector.size(); i++) {
        transactionVector.pop_back();
    }
    
    //set previous vector with all items pushed to the referenced vector
    transactionVector = transactionVector_;
}

//****************************************************************************************
//	GateControl::GetCardAuthorization
//****************************************************************************************
bool	GateControl::GetCardAuthorization(CardNumber number, Authorization& authorization)
{
    
    bool authorized = false;
    
    AuthorizationIterator it;
    it = authorizationMap_.find(number);
    //check if number is in map
    if (it == authorizationMap_.end()){
        //not found
        return authorized;
    } else {
        //place value in map at specified key
        authorization = authorizationMap_[number];
        authorized = true;
    }

	return authorized;
}

//*************************************************************************************
//	GateControl::GetCardTransactions
//****************************************************************************************

 bool	GateControl::GetCardTransactions(CardNumber number,
										 TransactionVector& transactionVector)
{
    bool status = false;
    
    //clear out passed vector
    for (int i = 0; i < transactionVector.size(); i++) {
        transactionVector.pop_back();
    }
    
    //load new items into vector
    for (int i = 0; i < transactionVector_.size(); i++) {
       Transaction trans1 = transactionVector_[i];
        if (trans1.number_ == number) {
            transactionVector.push_back(trans1);
            status = true;
        }
    }
	return status;
}



















