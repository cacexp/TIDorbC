//////////////////////////////////////////////////////////////////////////////////
//
// File:        SSLIOPCorbaloc.C
// Description: 
//
// Rel:         01.00
// Created:     September, 2009
// Author:      
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

#include "TIDorb/core/comm/ssliop.h"

#include <string>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::string;
#endif




char* TIDorb::core::comm::ssliop::SSLIOPCorbaloc::get_object_id(const char* corbaloc_URL,
                                                                const char* iface) // Never Called?
  throw(CORBA::ORB::InvalidName)
{
  string url = corbaloc_URL;

  string corbaloc_str;
  if (url.find(TIDorb::core::util::URL::SSLIOP_PROTOCOL) != string::npos) {
    corbaloc_str = url.substr(strlen(TIDorb::core::util::URL::SSLIOP_PROTOCOL));
  } else {
    throw CORBA::ORB::InvalidName();
  }

  TIDorb::core::comm::iiop::ListenPoint*  listen_point = NULL;
  TIDorb::core::comm::iiop::Version       version;

  // Look for slash
  string::size_type slash_position = corbaloc_str.find('/');
  string addr_str = corbaloc_str.substr(0, slash_position);
  
  string object_key_str = corbaloc_str.substr(slash_position + 1);
  bool other_addr = false;
  string next_str;

  do {
    // Look for a corbaloc composed by several listen points
    string::size_type next_addr_position = addr_str.find(",ssliop");
    other_addr = (next_addr_position != string::npos);
    
    if (other_addr) {
      next_str = addr_str.substr(next_addr_position+8);
      addr_str = addr_str.substr(0, next_addr_position);
    }

    // Look for version token
    string version_str, listenpoint_str;
    
    string::size_type at_position = addr_str.find('@');
    if (at_position == string::npos) {
      version_str = "1.0";
      listenpoint_str = addr_str;
    } else {
      version_str = addr_str.substr(0, at_position);
      listenpoint_str = addr_str.substr(at_position + 1);
    }

    // Parser version and listen_point
    version = TIDorb::core::comm::iiop::Corbaloc::parse_version(version_str);
    listen_point = TIDorb::core::comm::iiop::Corbaloc::parse_listenpoint(listenpoint_str, iface);

    delete listen_point;

    if (other_addr)
      addr_str = next_str;
  
  } while (other_addr);

  return CORBA::string_dup(object_key_str.c_str());
}




TIDorb::core::iop::IOR*
TIDorb::core::comm::ssliop::SSLIOPCorbaloc::get_IOR(const char* corbaloc_URL,
                                                    const char* iface)
  throw(CORBA::ORB::InvalidName)
{
  string url = corbaloc_URL;

  string corbaloc_str;
  if (url.find(TIDorb::core::util::URL::SSLIOP_PROTOCOL) != string::npos) {
    corbaloc_str = url.substr(strlen(TIDorb::core::util::URL::SSLIOP_PROTOCOL));
  } else {
    throw CORBA::ORB::InvalidName();
  }

  TIDorb::core::comm::iiop::ObjectKey*    objectkey = NULL;
  TIDorb::core::comm::iiop::ListenPoint*  listen_point = NULL;
  TIDorb::core::comm::iiop::Version       version;
  TIDorb::core::iop::VectorTaggedProfile  profiles;

  // Look for slash
  string::size_type slash_position = corbaloc_str.find('/');
  string addr_str = corbaloc_str.substr(0, slash_position);

  string object_key_str = corbaloc_str.substr(slash_position + 1);
  objectkey = create_key(object_key_str.c_str());
  TIDorb::core::iop::VectorTaggedComponent components;
  bool other_addr = false;
  string next_str;

  do {

    // Look for a corbaloc composed by several listen points
    string::size_type next_addr_position = addr_str.find(",ssliop");
    other_addr = (next_addr_position != string::npos);

    if (other_addr) {
      next_str = addr_str.substr(next_addr_position+8);
      addr_str = addr_str.substr(0, next_addr_position);
    }

    // Look for version token
    string version_str, listenpoint_str;
    
    string::size_type at_position = addr_str.find('@');
    if (at_position == string::npos) {
      version_str = "1.0";
      listenpoint_str = addr_str;
    } else {
      version_str = addr_str.substr(0, at_position);
      listenpoint_str = addr_str.substr(at_position + 1);
    }

    // Parser version and listen_point
    version = TIDorb::core::comm::iiop::Corbaloc::parse_version(version_str);
    listen_point = TIDorb::core::comm::iiop::Corbaloc::parse_listenpoint(listenpoint_str, iface);

    // Add to components a new SSLComponent with a listenpoint port as SSL
    SSLIOP::SSL ssl(CSIIOP::Integrity | CSIIOP::Confidentiality |  CSIIOP::NoDelegation,
                    CSIIOP::Integrity | CSIIOP::Confidentiality |  CSIIOP::NoDelegation,
                    listen_point->_port);
    TIDorb::core::comm::ssliop::SSLComponent* ssl_component = 
      new TIDorb::core::comm::ssliop::SSLComponent(ssl);
    components.resize(1);
    components[0] = ssl_component;

    listen_point->_ssl_port = listen_point->_port;
    listen_point->_port = 0; // not used

    // Create Profiles
    profiles.push_back(new TIDorb::core::comm::iiop::ProfileIIOP(version, *listen_point,
                                                                 objectkey, components));
    delete listen_point;

    if (other_addr) 
      addr_str = next_str;
  
  } while (other_addr);

  return new TIDorb::core::iop::IOR("IDL:omg.org/CORBA/Object:1.0", &profiles);
}




TIDorb::core::comm::iiop::ObjectKey*
TIDorb::core::comm::ssliop::SSLIOPCorbaloc::create_key(const char* url)
  throw(CORBA::ORB::InvalidName)
{
  CORBA::ULong url_len = strlen(url);
  TIDorb::core::cdr::BufferCDR* buffer = 
    new TIDorb::core::cdr::BufferCDR(url_len +
                                     TIDorb::core::cdr::CDR::ULONG_SIZE);

  TIDorb::core::cdr::CDROutputStream output (NULL, buffer);

  output.write_ulong(url_len); 
  output.write_octet_array((CORBA::Octet*)url,0,url_len); 

  TIDorb::core::cdr::CDRInputStream * input =
    (TIDorb::core::cdr::CDRInputStream*) (output.create_input_stream());

  TIDorb::core::cdr::CDRInputStream * key_input = input->read_encapsulation();

  delete input;

  return new TIDorb::core::comm::iiop::ObjectKey(key_input);
}
