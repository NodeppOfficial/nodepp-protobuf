# Nodepp Protobuf

[![MIT License](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Nodepp](https://img.shields.io/badge/Nodepp-%3E%3D1.4.7-blue)](https://github.com/NodeppOfficial/nodepp)

**A minimal, zero-dependency Protocol Buffers implementation for Nodepp.**  
Parse and serialize protobuf messages in ~100 lines of C++. No code generation. No `protoc`. No dependencies.

Built on Nodepp's `ptr_t`, `any_t`, and `array_t`. Zero-copy. Thread-safe. 2KB.

---

## 🚀 Quick Start

```cpp
#include <nodepp/nodepp.h>
#include <protobuf/protobuf.h>

using namespace nodepp;

void onMain() {

    // Build a protobuf message
    protobuf_t msg;
    msg.push( uchar_16(42)     );  // field 0: varint
    msg.push( string_t("hola") );  // field 1: length-delimited
    msg.push( uchar_32(12345)  );  // field 2: 32-bit
    msg.push( uchar_64(12345)  );  // field 3: 64-bit

    // Serialize
    auto data = protobuf::format( msg );
    console::log( "encoded:", data.size(), "bytes" );

    // Parse
    auto parsed = protobuf::parse( data );
    console::log( "field 0:", parsed[0].as<uchar_16>() );
    console::log( "field 1:", parsed[1].as<string_t>() );
    console::log( "field 2:", parsed[2].as<uchar_32>() );
    console::log( "field 4:", parsed[3].as<uchar_64>() );

}

```