//////////////////////////////////////////////////////////////////////////////////
//
// File:        TaggedProfile.h
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/iop.h"

#ifndef _TIDORB_CORE_IOP_TAGGEDPROFILE_H_
#define _TIDORB_CORE_IOP_TAGGEDPROFILE_H_

namespace TIDorb {
namespace core {
namespace iop {

class TaggedProfile : public TIDThr::RecursiveMutex
{
public:
        CORBA::ULong _tag;

        TaggedProfile(CORBA::ULong tag) throw (TIDThr::SystemException);
        virtual ~TaggedProfile() throw (TIDThr::SystemException){};

	virtual bool operator== (const TIDorb::core::iop::TaggedProfile& other) const = 0;

        void write(TIDorb::core::cdr::CDROutputStream& output) const;

        virtual void partial_write(TIDorb::core::cdr::CDROutputStream& output) const = 0;
        virtual void partial_read(TIDorb::core::cdr::CDRInputStream& input) = 0;

        static void skip(TIDorb::core::cdr::CDRInputStream& input);


};
}// iop
}// core
}// TIDorb
#endif

