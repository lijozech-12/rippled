//
// packaging of messages into length/type-prepended buffers
// ready for transmission.

#ifndef PACKEDMESSAGE_H
#define PACKEDMESSAGE_H

#include <string>
#include <cassert>
#include <vector>
#include <cstdio>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/cstdint.hpp>
#include "newcoin.pb.h"

// The header size for packed messages
// len(4)+type(2)
const unsigned HEADER_SIZE = 6;


// PackedMessage implements simple "packing" of protocol buffers Messages into
// a string prepended by a header specifying the message length.
// MessageType should be a Message class generated by the protobuf compiler.
//

class PackedMessage : public boost::enable_shared_from_this<PackedMessage>
{
	// Encodes the size and type into a header at the beginning of buf
	//
	void encodeHeader(unsigned size, int type);


	std::vector<uint8_t> mBuffer;
public:
    typedef boost::shared_ptr<::google::protobuf::Message> MessagePointer;
	typedef boost::shared_ptr<PackedMessage> pointer;

	MessagePointer mMsg;
    PackedMessage(MessagePointer msg,int type);

	std::vector<uint8_t>& getBuffer(){ return(mBuffer); }

	static unsigned getLength(std::vector<uint8_t>& buf);
	static int getType(std::vector<uint8_t>& buf);

	bool operator == (const PackedMessage& other);

	/*
    void setMsg(MessagePointer msg,int type);

    MessagePointer getMsg();

    // Pack the message into the given data_buffer. The buffer is resized to
    // exactly fit the message.
    // Return false in case of an error, true if successful.
    //
    bool pack(data_buffer& buf) const;

    // Given a buffer with the first HEADER_SIZE bytes representing the header,
    // decode the header and return the message length. Return 0 in case of 
    // an error.
    //
    unsigned decodeHeader(const data_buffer& buf) const;

    // Unpack and store a message from the given packed buffer.
    // Return true if unpacking successful, false otherwise.
    //
    bool unpack(const data_buffer& buf);
	*/
};

#endif /* PACKEDMESSAGE_H */

