/////////////////////////////////////////////////////////////////////////
//
// File:        DynStructImpl.h
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

#include "TIDorb/dynAny.h"


#ifndef __TIDORB_DYN_STRUCT_IMPL_H__
#define __TIDORB_DYN_STRUCT_IMPL_H__


namespace TIDorb {
namespace dynAny {


class DynStructImpl : public DynamicAny::DynStruct,
                      public CORBA::LocalObject,
                      public TIDThr::RefCounter, 
                      public DynComposite
{
   public:
      DynamicAny::DynAny_ptr copy()
         throw(CORBA::SystemException);

      // DynStruct operations
      virtual DynamicAny::FieldName current_member_name()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      virtual CORBA::TCKind current_member_kind()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      virtual DynamicAny::NameValuePairSeq* get_members()
         throw(CORBA::SystemException);

      virtual void set_members(const DynamicAny::NameValuePairSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      virtual DynamicAny::NameDynAnyPairSeq* get_members_as_dyn_any()
         throw(CORBA::SystemException);

      virtual void set_members_as_dyn_any(
         const DynamicAny::NameDynAnyPairSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      // LocalObject operations
      CORBA::Boolean _is_a(const char* repositoryIdentifier)
         throw(CORBA::SystemException);

   public:
      // Destructor
      virtual ~DynStructImpl()
        throw (TIDThr::SystemException);

   protected:
      // Empty Constructor for generate copies
      DynStructImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb)
         throw(CORBA::SystemException);

      // Constructor. Gets an any object for reading its value.
      // It assumes that the any contains an struct TypeCode.
      DynStructImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                    const CORBA::Any& any, CORBA::TypeCode_ptr real_type)
         throw(CORBA::SystemException);

      // Constructor. Gets a simple TypeCode to create a new value.
      // It assumes that the TypeCode is tk_struct or tk_exception
      DynStructImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                    CORBA::TypeCode_ptr type, CORBA::TypeCode_ptr real_type)
         throw(CORBA::SystemException);

      CORBA::TypeCode_ptr get_component_type(CORBA::ULong position)
         throw(CORBA::SystemException);

   friend class DynAnyFactoryImpl;
};


} //namespace dynAny
} //namespace TIDorb


#endif //__TIDORB_DYN_STRUCT_IMPL_H__