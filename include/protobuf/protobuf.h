/*
 * Copyright 2023 The NodeppOfficial Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOfficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_PROTOBUF
#define NODEPP_PROTOBUF

/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/nodepp.h>
#include <nodepp/object.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { using protobuf_t = array_t<object_t>; }

namespace nodepp { namespace protobuf {

    inline protobuf_t parse( string_t data ){
    /*--*/ protobuf_t out; ulong x=0; 
        
        while( x<data.size() ){ switch( data[x] & 0x07 ){

            case 0: do { uchar_16 tmp=0; x++; while( data[x] & 0x80 ){
            if( x > data.size() ){ return nullptr; }
                tmp |= data[x] & 0x7F; x++; tmp <<= 7;
            }   tmp |= data[x] & 0x7F; x++;
            
                tmp = encoder::endian::to_big_endian( tmp );
                out.push( tmp ); 
            
            } while(0); break;

            case 5: do { uchar_32 tmp=0; x++;
            if( x + sizeof(uchar_32) > data.size() ){ return nullptr; }

                memcpy( &tmp, data.get()+x, sizeof(uchar_32) );
                tmp = encoder::endian::to_big_endian( tmp );
                out.push( tmp ); x += sizeof( uchar_32 );
                
            } while(0); break;

            case 1: do { uchar_64 tmp=0; x++;
            if( x + sizeof(uchar_64) > data.size() ){ return nullptr; }

                memcpy( &tmp, data.get()+x, sizeof(uchar_64) );
                tmp = encoder::endian::to_big_endian( tmp );
                out.push( tmp ); x += sizeof( uchar_64 );

            } while(0); break;

            case 2: do { uchar_16 len=0; x++; while( data[x] & 0x80 ){
            if( x > data.size() ){ return nullptr; }
                len |= data[x] & 0x7F; x++; len <<= 7;
            }   len |= data[x] & 0x7F; x++;
            
                len = encoder::endian::to_big_endian( len );
                string_t tmp ( data.get()+x, len ); 
                out.push( tmp ); x+=len;
        
            } while(0); break;

            default: return out; break;

        }}

    return out; }

    inline string_t format( protobuf_t data ){
    /*--*/ queue_t<string_t> out; uchar y = 0;

        for( auto x: data ){ if( x.has_value() ){ do {
            
            if( x.type_size() == sizeof(uchar_8) ){ 
                out.push( string_t( 1, y << 3 | 0x00 ) );
                auto len = queue_t<char>();
                auto val = encoder::endian::to_big_endian( x.as<uchar_8>() );
            do {

                uchar  tmp = 0x00;
                tmp  = val & 0x7F; val>>=7; 
                len.unshift( tmp | 0x80 );

            } while( val!=0 );
                len[len.size()-1] &=~ 0x80;
                out.push( string::join( len, "" ) ); 
            }

            elif( x.type_size() == sizeof(uchar_16) ){ 
                out.push( string_t( 1, y << 3 | 0x00 ) );
                auto len = queue_t<char> ();
                auto val = encoder::endian::to_big_endian( x.as<uchar_16>() );
            do {

                uchar  tmp = 0x00;
                tmp  = val & 0x7F; val>>=7; 
                len.unshift( tmp | 0x80 );

            } while( val!=0 );
                auto tmp = string::join( len, "" );
                tmp[tmp.size()-1] &=~ 0x80; out.push( tmp ); 
            }

            elif( x.type_size() == sizeof(uchar_32) ){
                out.push( string_t( 1, y << 3 | 0x05 ) );
            do {
                string_t tmp( sizeof( uchar_32 ), '\0' );
                auto mem = (uchar_32*) tmp.get();
                mem[0]   = encoder::endian::to_big_endian( x.as<uchar_32>() );
                out.push( tmp ); 
            } while(0); }

            elif( x.type_size() == sizeof(uchar_64) ){
                out.push( string_t( 1, y << 3 | 0x01 ) );
            do {
                string_t tmp( sizeof( uchar_64 ), '\0' );
                auto mem = (uchar_64*) tmp.get();
                mem[0]   = encoder::endian::to_big_endian( x.as<uchar_64>() );
                out.push( tmp );
            } while(0); }

            elif( x.is<string_t>() ){ 
                out.push( string_t( 1, y << 3 | 0x02 ) );
                auto len = queue_t<char> ();
                auto raw = uchar_16( x.as<string_t>().size() );
                auto val = encoder::endian::to_big_endian( raw );
            do {

                uchar  tmp = 0x00;
                tmp  = val & 0x7F; val>>=7; 
                len.unshift( tmp | 0x80 );

            } while( val!=0 );
                auto tmp = string::join( len, "" );
                tmp[ tmp.size() - 1 ] &=~ 0x80;
                out.insert( nullptr, { tmp, x.as<string_t>().slice(0,raw) } );
            }

        } while(0); y++; }}

    return string::join( out, "" ); }

} }

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/