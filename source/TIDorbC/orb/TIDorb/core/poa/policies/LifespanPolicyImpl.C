/////////////////////////////////////////////////////////////////////////
//
// File:        LifespanPolicyImpl.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:
//
// (C) Copyright 2009 Telefonica Investigacion y Desarrollo
//     S.A.Unipersonal (Telefonica I+D)
//
// This file is part of Morfeo CORBA Platform.
//
// Morfeo CORBA Platform is free software: you can redistribute it and/or
// modify it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// Morfeo CORBA Platform is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Morfeo CORBA Platform. If not, see
//
//   http://www.gnu.org/licenses
//
// Info about members and contributors of the MORFEO project
// is available at
//
//   http://morfeo-project.org
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/poa/policies.h"

TIDorb::core::poa::policies::LifespanPolicyImpl::LifespanPolicyImpl(PortableServer::LifespanPolicyValue value) 
{
  destroyed = false; 
  _value = value;
};

TIDorb::core::poa::policies::LifespanPolicyImpl::~LifespanPolicyImpl() {
};
    
PortableServer::LifespanPolicyValue TIDorb::core::poa::policies::LifespanPolicyImpl::value() throw() {
    	return _value;
};
  
CORBA::PolicyType TIDorb::core::poa::policies::LifespanPolicyImpl::policy_type() throw() {
    	return PortableServer::LIFESPAN_POLICY_ID;
};
  
CORBA::Policy_ptr TIDorb::core::poa::policies::LifespanPolicyImpl::copy() throw() {
  return new LifespanPolicyImpl(_value);
};
  	
CORBA::Boolean TIDorb::core::poa::policies::LifespanPolicyImpl::_is_a(const char* repositoryIdentifier) {
	if(repositoryIdentifier == NULL)
		throw CORBA::BAD_PARAM("Null string reference");

	if(strcmp(repositoryIdentifier,"IDL:omg.org/PortableServer/LifespanPolicy:1.0")==0)
		return true;
	
	return ::CORBA::LocalObject::_is_a(repositoryIdentifier);

};
  
CORBA::Boolean TIDorb::core::poa::policies::LifespanPolicyImpl::_is_equivalent(CORBA::Object_ptr other)
{
  	if(other == NULL)
		throw CORBA::BAD_PARAM("Null Object reference");
		
	PortableServer::LifespanPolicy* aux_policy = dynamic_cast<PortableServer::LifespanPolicy*>(other);
	
	if (aux_policy)
		return true;
	else 
		return false;	
			
	
};

void TIDorb::core::poa::policies::LifespanPolicyImpl::destroy() throw() {
};

