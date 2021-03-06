/////////////////////////////////////////////////////////////////////////
//
// File:        NVListImpl.C
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

#include "TIDorb/core.h"

#include <typeinfo>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif



TIDorb::core::NVListImpl::NVListImpl(TIDorb::core::TIDORB* orb)  throw (TIDThr::SystemException)
  : m_orb(orb)
{
  __ref_count_ = 1;
}

TIDorb::core::NVListImpl::~NVListImpl()  throw (TIDThr::SystemException)
{
  ItemList::iterator i;
  ItemList::iterator end = m_list.end();
  
 for(i = m_list.begin(); i != end; i++)
    delete (*i);
}

CORBA::ULong TIDorb::core::NVListImpl::count() const
{
  return m_list.size();
}

CORBA::NamedValue_ptr TIDorb::core::NVListImpl::add(CORBA::Flags flags)
{
  NamedValueImpl* nv = new NamedValueImpl(NULL, false, m_orb->create_any(), flags);
  m_list.push_back(nv);
  return (nv); 
}

CORBA::NamedValue_ptr TIDorb::core::NVListImpl::add_item(const char* item_name,
                                                         CORBA::Flags flags)
{
  if(!item_name)
    throw CORBA::BAD_PARAM();
  CORBA::Any* new_any = m_orb->create_any();
  NamedValueImpl* nv = new NamedValueImpl((char*)item_name, false, 
                                          new_any, flags);

  m_list.push_back(nv);
  return (nv); 
}

CORBA::NamedValue_ptr TIDorb::core::NVListImpl::add_value(const char* item_name, 
                                                          const CORBA::Any& value, 
                                                          CORBA::Flags flags)
{
  if(!item_name)
    throw CORBA::BAD_PARAM();
  
  CORBA::Any* new_value = new CORBA::Any(value);

  NamedValueImpl* nv = new NamedValueImpl((char*)item_name, false, new_value, flags);
  m_list.push_back(nv);
  return (nv);
}

CORBA::NamedValue_ptr TIDorb::core::NVListImpl::add_item_consume(char* item_name, 
                                                                 CORBA::Flags flags)
{
  if(!item_name)
    throw CORBA::BAD_PARAM();
  
  NamedValueImpl* nv = new NamedValueImpl(item_name, true, m_orb->create_any(), flags);
  m_list.push_back(nv);
  return (nv);
}

CORBA::NamedValue_ptr TIDorb::core::NVListImpl::add_value_consume(char* item_name,CORBA::Any* value, CORBA::Flags flags)
{
  if((!item_name) || (!value))
    throw CORBA::BAD_PARAM();
  
  NamedValueImpl* nv = new NamedValueImpl(item_name, true, value, flags);
  m_list.push_back(nv);
  return (nv); 
}

CORBA::NamedValue_ptr TIDorb::core::NVListImpl::item(CORBA::ULong index)
{
  if(index >= m_list.size())
    throw CORBA::BAD_PARAM();
  
  return m_list[index];
}

void TIDorb::core::NVListImpl::remove(CORBA::ULong index)
{
  CORBA::ULong size = m_list.size();
  
  if(index >= size)
    throw CORBA::BAD_PARAM();
  
  CORBA::release(m_list[index]);
  
  for(CORBA::ULong i = index; i < size; i++) 
    m_list[i] = m_list[i+1];
  
  m_list.resize(size - 1);
}


CORBA::NVList_ptr CORBA::NVList::_duplicate(CORBA::NVList_ptr list)
{
  
  try {
    
    TIDorb::core::NVListImpl* list_impl = NULL;
    
    if (list)
      list_impl = (TIDorb::core::NVListImpl*)list->_impl();
    
    if(list_impl)
      list_impl->_add_ref();
    
    return list;
    
  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL();
  }
   
  return list;
}

CORBA::NVList_ptr CORBA::NVList::_nil()
{
  return 0;
}

void CORBA::release(CORBA::NVList_ptr list)
{
  
  try {
    
    TIDorb::core::NVListImpl* list_impl = NULL;
    
    if (list)
      list_impl = (TIDorb::core::NVListImpl*)list->_impl();
    
    if(list_impl)
      list_impl->_remove_ref();
    
  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL();
  }
   
}

CORBA::Boolean CORBA::is_nil(CORBA::NVList_ptr list)
{
  return !list;
}


void TIDorb::core::NVListImpl::assign_out_arguments(CORBA::NVList_ptr from_list, 
                                                    CORBA::NVList_ptr to_list)
{
  assign_out_arguments(from_list, to_list, false);
}

void TIDorb::core::NVListImpl::assign_out_arguments(CORBA::NVList_ptr from_list, 
                                                    CORBA::NVList_ptr to_list,
                                                    bool wrap_anys)
{
  CORBA::ULong length = to_list->count(); 
  
  if (length < from_list->count())
    throw CORBA::MARSHAL (); //"Invalid number of out arguments.",
  //0, CompletionStatus.COMPLETED_NO);  
  
  CORBA::NamedValue * to_nam_val;
  CORBA::NamedValue * from_nam_val;		
  CORBA::TypeCode_var tc_type;
  try {
    for (CORBA::ULong i = 0; i < length; i++) {
      to_nam_val = to_list->item(i);
      from_nam_val = from_list->item(i);
      if(to_nam_val->flags() != CORBA::ARG_IN) { 
        AnyImpl& from_any = *(AnyImpl*) &(from_nam_val->value()->delegate());
        AnyImpl& to_any =   *(AnyImpl*) & (to_nam_val->value()->delegate());        
        tc_type = from_any.type();
        to_any.set_type(tc_type);
        to_any.assign_value(from_any, wrap_anys);
      }
    }
    
  } catch (const bad_cast& bc) {
    throw CORBA::INTERNAL();//"Bad NVList");
  }
  
}

void TIDorb::core::NVListImpl::assign_in_arguments(CORBA::NVList_ptr from_list, 
                                                   CORBA::NVList_ptr to_list)
{
  assign_in_arguments(from_list, to_list, false);
}

void TIDorb::core::NVListImpl::assign_in_arguments(CORBA::NVList_ptr from_list, 
                                                   CORBA::NVList_ptr to_list,
                                                   bool wrap_anys)
{
  CORBA::ULong length = to_list->count(); 
  
  if (length < from_list->count())
    throw CORBA::MARSHAL ("Invalid number of out arguments.");
  //0, CompletionStatus.COMPLETED_NO);  
  
  CORBA::NamedValue * to_nam_val;
  CORBA::NamedValue * from_nam_val;		
  CORBA::TypeCode_var tc_type;
  try {
    for (CORBA::ULong i = 0; i < length; i++) {
      to_nam_val = to_list->item(i);
      from_nam_val = from_list->item(i);
      if(to_nam_val->flags() != CORBA::ARG_OUT){ 
        AnyImpl& from_any = *(AnyImpl*) & (from_nam_val->value()->delegate());
        AnyImpl& to_any = *(AnyImpl*) &(to_nam_val->value()->delegate());
        tc_type = from_any.type();
        to_any.set_type(tc_type);
        to_any.assign_value(from_any, wrap_anys);
      }
    }
    
  } catch (const bad_cast& bc) {
    throw CORBA::INTERNAL();//"Bad NVList");
  }
  
}


void TIDorb::core::NVListImpl::read_params_out(CORBA::NVList_ptr list, TIDorb::core::cdr::CDRInputStream& input)
{
  if(!(list))
    throw CORBA::BAD_PARAM("Null list reference");
    
  CORBA::ULong length = list->count();
  
  CORBA::NamedValue * nam_val;
  CORBA::TypeCode_var tc;
  
  
  for (CORBA::ULong i = 0; i < length; i++) {
    nam_val = list->item(i);
    
    if(nam_val->flags() != CORBA::ARG_IN) {
      tc = nam_val->value()->type();
      nam_val->value()->delegate().read_value(input,tc);
    }
  }
}

void TIDorb::core::NVListImpl::write_params_out(CORBA::NVList_ptr list, TIDorb::core::cdr::CDROutputStream& output)
{
  if(!(list))
    throw CORBA::BAD_PARAM("Null list reference");
    
  CORBA::ULong length = list->count();
  
  CORBA::NamedValue * nam_val;
  
  for (CORBA::ULong i = 0; i < length; i++) {
    nam_val = list->item(i);
    
    if(nam_val->flags() != CORBA::ARG_IN) {
      nam_val->value()->delegate().write_value(output);
    }
  }
}

void TIDorb::core::NVListImpl::read_params_in(CORBA::NVList_ptr list, TIDorb::core::cdr::CDRInputStream& input)
{
 if(!(list))
    throw CORBA::BAD_PARAM("Null list reference");
    
  CORBA::ULong length = list->count();
  
  CORBA::NamedValue * nam_val;
  CORBA::TypeCode_var tc;
  
  
  for (CORBA::ULong i = 0; i < length; i++) {
    nam_val = list->item(i);
    
    if(nam_val->flags() != CORBA::ARG_OUT) {
      tc = nam_val->value()->type();
      nam_val->value()->delegate().read_value(input,tc);
    }
  }
}

void TIDorb::core::NVListImpl::write_params_in(CORBA::NVList_ptr list, TIDorb::core::cdr::CDROutputStream& output)
{
  if(!(list))
    throw CORBA::BAD_PARAM("Null list reference");
    
  CORBA::ULong length = list->count();

  CORBA::NamedValue * nam_val;
  
  for (CORBA::ULong i = 0; i < length; i++) {
    nam_val = list->item(i);
    
    if(nam_val->flags() != CORBA::ARG_OUT) {
      nam_val->value()->delegate().write_value(output);
    }
  }
}


void* TIDorb::core::NVListImpl::_impl()
{
  return this;
}

const char* TIDorb::core::NVListImpl::_typeid()
{
  //return CORBA::string_dup("NVListImpl");
  return "NVListImpl";
}
