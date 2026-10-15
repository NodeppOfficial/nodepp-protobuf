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