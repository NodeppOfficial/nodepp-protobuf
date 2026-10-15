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

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { using protobuf_t = array_t<any_t>; }

namespace nodepp { namespace protobuf {

    inline protobuf_t parse( string_t data ){
    /*--*/ protobuf_t out; ulong x=0; 
        
        while( x<data.size() ){ switch( data[x] & 0x07 ){

            case 0: do { uchar_16 tmp=0; x++; while( data[x] & 0x80 ){
            if( x > data.size() ){ return nullptr; }
                tmp |= data[x] & 0x7F; x++; tmp <<= 7;
            }   tmp |= data[x] & 0x7F; x++; out.push( tmp ); 
            } while(0); break;

            case 5: do { uchar_32 tmp=0; x++;
            if( x + sizeof(uchar_32) > data.size() ){ return nullptr; }

                memcpy( &tmp, data.get()+x, sizeof(uchar_32) );
                out.push( tmp ); x += sizeof( uchar_32 );
                
            } while(0); break;

            case 1: do { uchar_64 tmp=0; x++;
            if( x + sizeof(uchar_64) > data.size() ){ return nullptr; }

                memcpy( &tmp, data.get()+x, sizeof(uchar_64) );
                out.push( tmp ); x += sizeof( uchar_64 );

            } while(0); break;

            case 2: do { uchar_16 len=0; x++; while( data[x] & 0x80 ){
            if( x > data.size() ){ return nullptr; }
                len |= data[x] & 0x7F; x++; len <<= 7;
            }   len |= data[x] & 0x7F; x++;
            
                string_t tmp ( data.get()+x, len ); 
                out.push( tmp ); x+=len;
        
            } while(0); break;

            default: return out; break;

        }}

    return out; }

    inline string_t format( protobuf_t data ){
    /*--*/ queue_t<string_t> out; uchar y = 0;

        for( auto x: data ){ if( x.has_value() ){ do {
            
            if( x.is<uchar_8>() ){ 
                out.push( string_t( 1, y << 3 | 0x00 ) );
                auto val = x.as<uchar_8> ();
            if   ( val==0 ){ out.push( string_t( 1, 0x00 ) ); break; }
            while( val!=0 ){

                string_t tmp(1,'\0'); 
                tmp[0] =val & 0x7F;val>>=7; 
                tmp[0]|=val== 0? 0x00:0x80;

            out.push( tmp ); }}

            elif( x.is<uchar_16>() ){ 
                out.push( string_t( 1, y << 3 | 0x00 ) );
                auto len = queue_t<char> ();
                auto val = x.as<uchar_16>();
            if   ( val==0 ){ out.push( string_t( 1, 0x00 ) ); break; }
            while( val!=0 ){

                char   tmp = 0x00;
                tmp  = val & 0x7F;val>>=7; 
                tmp |= val== 0? 0x00:0x80;
                len.push( tmp );

            } out.push( string::join( len, "" ) ); }

            elif( x.is<uchar_32>() ){
                out.push( string_t( 1, y << 3 | 0x05 ) );
            do {
                string_t tmp( sizeof( uchar_32 ), '\0' );
                auto mem = (uchar_32*) tmp.get();
                mem[0]   = x.as<uchar_32>();
                out.push( tmp ); 
            } while(0); }

            elif( x.is<uchar_64>() ){
                out.push( string_t( 1, y << 3 | 0x01 ) );
            do {
                string_t tmp( sizeof( uchar_64 ), '\0' );
                auto mem = (uchar_64*) tmp.get();
                mem[0]   = x.as<uchar_64>();
                out.push( tmp );
            } while(0); }

            elif( x.is<string_t>() ){ 
                out.push( string_t( 1, y << 3 | 0x02 ) );
                auto val = x.as<string_t>().size();
                auto len = queue_t<char> ();
            if   ( val==0 ){ out.push( string_t( 1, 0x00 ) ); break; }
            while( val!=0 ){
                
                char   tmp = 0x00;
                tmp  = val & 0x7F;val>>=7; 
                tmp |= val== 0? 0x00:0x80;
                len.push( tmp );
            
            }
                out.push( string::join( len, "" ) );
                out.push( x.as<string_t>() ); 
            }

        } while(0); y++; }}

    return string::join( out, "" ); }

} }

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/