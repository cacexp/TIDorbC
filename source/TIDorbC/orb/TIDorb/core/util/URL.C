/////////////////////////////////////////////////////////////////////////
//
// File:        URL.C
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

#include "TIDorb/core/util/URL.h"


const char* TIDorb::core::util::URL::CORBANAME_PROTOCOL = "corbaname:";
const char* TIDorb::core::util::URL::FILE_PROTOCOL      = "file://";
const char* TIDorb::core::util::URL::RIR_PROTOCOL       = "corbaloc:rir:";
const char* TIDorb::core::util::URL::TIDORB_PROTOCOL    = "corbaloc:tidorb:";
const char* TIDorb::core::util::URL::IIOP_PROTOCOL_1    = "corbaloc::";
const char* TIDorb::core::util::URL::IIOP_PROTOCOL_2    = "corbaloc:iiop:";

// MIOP extensions
const char* TIDorb::core::util::URL::MIOP_PROTOCOL      = "corbaloc:miop:";
// end MIOP extensions

const char* TIDorb::core::util::URL::SSLIOP_PROTOCOL      = "corbaloc:ssliop:";
