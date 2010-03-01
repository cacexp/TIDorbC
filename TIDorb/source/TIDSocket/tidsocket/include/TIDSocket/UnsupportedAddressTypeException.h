///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_UnsupportAddressTypeException.h
// Description: This file contains the UnsupportAddressTypeException class methods
// Rel:         01.00
// Created:     May, 2001
// Author:      Pablo Rodriguez Archilla      ( pra@tid.es   )
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
///////////////////////////////////////////////////////////////////////////

#include "TIDSocket.h"


#ifndef __TID_SOCKET_UNSUPPORTED_ADDRESS_TYPE_EXCEPTION_H__
#define __TID_SOCKET_UNSUPPORTED_ADDRESS_TYPE_EXCEPTION_H__


namespace TIDSocket
{


class UnsupportedAddressTypeException : public IllegalArgumentException
{
    public:
        UnsupportedAddressTypeException(const char* msg = "", int errnum = 0)
            throw() : IllegalArgumentException(msg, errnum) {}

        virtual ~UnsupportedAddressTypeException()
            throw() {}

        const char* getName() const
            throw() { return "TIDSocket::UnsupportedAddressTypeException"; }
};


} //namespace TIDSocket


#endif //__TID_SOCKET_UNSUPPORTED_ADDRESS_TYPE_EXCEPTION_H__