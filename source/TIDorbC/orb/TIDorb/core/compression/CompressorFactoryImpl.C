//////////////////////////////////////////////////////////////////////////////////
//
// File:        CompressorFactoryImpl.C
// Description: Compression::CompressorFactory implementation.
//
// Rel:         01.00
// Created:     October, 2007
// Author:      Alvaro Vega Garcia      ( avega@tid.es   )
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

#include "TIDorb/core/compression/CompressorFactoryImpl.h"


TIDorb::core::compression::CompressorFactoryImpl::CompressorFactoryImpl(
                                                        TIDorb::core::TIDORB* orb,
                                                        Compression::CompressorId id) 
  throw (TIDThr::SystemException)
  : m_orb(orb), m_compressor_id(id)
{
  this->TIDThr::RefCounter::_add_ref();

  switch (m_compressor_id) {
  case Compression::COMPRESSORID_ZLIB:
    break;
  default:
    throw CORBA::NO_IMPLEMENT();
  }
}

  
TIDorb::core::compression::CompressorFactoryImpl::~CompressorFactoryImpl() 
  throw (TIDThr::SystemException)
{ 
  m_compressors.clear();
}



Compression::CompressorId 
TIDorb::core::compression::CompressorFactoryImpl::compressor_id()
{ 
  return m_compressor_id;
}


// CORBA::ULongLong 
// TIDorb::core::compression::CompressorFactoryImpl::compressed_bytes()
// { 
//   return m_compressed_bytes;
// }  


// CORBA::ULongLong 
// TIDorb::core::compression::CompressorFactoryImpl::uncompressed_bytes()
// { 
//   return m_uncompressed_bytes;
// }


// CORBA::Double TIDorb::core::compression::CompressorFactoryImpl::average_compression()
// { 
//   return 0;
// }

Compression::Compressor_ptr 
TIDorb::core::compression::CompressorFactoryImpl::get_compressor(
                                     Compression::CompressionLevel compression_level)
{ 
  // Check level
  if (compression_level > 9)
    throw CORBA::BAD_PARAM("Compression level not valid", 44, 
                           CORBA::COMPLETED_NO);


  // Check if already exists
  CompressorMapT::iterator it = m_compressors.find(compression_level);
   
  if (it != m_compressors.end()){

    return Compression::Compressor::_duplicate(m_compressors[compression_level]);

  } else {

    Compression::Compressor_ptr new_compressor;

    switch (m_compressor_id) {
    case Compression::COMPRESSORID_GZIP:
    case Compression::COMPRESSORID_PKZIP:
    case Compression::COMPRESSORID_BZIP2:
      throw CORBA::NO_IMPLEMENT();
    case Compression::COMPRESSORID_ZLIB:
      new_compressor = new TIDorb::core::compression::ZlibCompressorImpl(m_orb, 
                                                                         compression_level, 
                                                                         this);
      break;
    default:
      throw CORBA::NO_IMPLEMENT();
    }
  
    m_compressors[compression_level] = Compression::Compressor::_duplicate(new_compressor);

    return new_compressor;
  }
}


// void TIDorb::core::compression::CompressorFactoryImpl::add_sample(
//                                            CORBA::ULongLong compressed_bytes, 
//                                            CORBA::ULongLong uncompressed_bytes)
// { 
//   m_compressed_bytes   += compressed_bytes;
//   m_uncompressed_bytes += uncompressed_bytes;
// }
