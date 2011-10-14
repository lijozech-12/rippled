#ifndef __PEER__
#define __PEER__

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "newcoin.pb.h"
#include "PackedMessage.h"
#include "Ledger.h"
#include "list"

class KnownNode;

/*
This is one other node you are connected to.
When you connect you:
	Send Hello
	Send Your latest ledger

*/

class Peer : public boost::enable_shared_from_this<Peer>
{
	// Must keep track of the messages you have already sent to or received from this peer
	// Well actually we can just keep track of if we have broadcast each message

	boost::asio::ip::tcp::socket mSocket;
	std::vector<uint8_t> mReadbuf;
	std::list<PackedMessage::pointer> mSendQ;
	PackedMessage::pointer mSendingPacket;


	Peer(boost::asio::io_service& io_service);

	void handle_write(const boost::system::error_code& error, size_t bytes_transferred);
	//void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_read_header(const boost::system::error_code& error);
	void handle_read_body(const boost::system::error_code& error);
	void processReadBuffer();
	void start_read_header();
	void start_read_body(unsigned msg_len);

	
	void sendPacketForce(PackedMessage::pointer packet);


	void sendHello();
	void sendTransaction();
	void sendValidation();
	void receiveHello(newcoin::Hello& packet);
	void receiveTransaction(newcoin::Transaction& packet);
	void receiveValidation(newcoin::Validation& packet);
	void receiveFullLedger(newcoin::FullLedger& packet);
	void receiveProposeLedger(newcoin::ProposeLedger& packet);
	void receiveGetFullLedger(newcoin::GetFullLedger& packet);
	void receiveGetValidations(newcoin::GetValidations& packet);

public:
	typedef boost::shared_ptr<Peer> pointer;

	//bool operator == (const Peer& other);

	static pointer create(boost::asio::io_service& io_service)
	{
		return pointer(new Peer(io_service));
	}

	boost::asio::ip::tcp::socket& getSocket()
	{
		return mSocket;
	}

	void connected(const boost::system::error_code& error);

	// try to connect to this Peer
	void connectTo(KnownNode& node);

	void sendPacket(PackedMessage::pointer packet);
	void sendLedgerProposal(Ledger::pointer ledger);
	void sendFullLedger(Ledger::pointer ledger);
	void sendGetFullLedger(uint64 index);

	//static PackedMessage::pointer createFullLedger(Ledger::pointer ledger);
	static PackedMessage::pointer createLedgerProposal(Ledger::pointer ledger);
	static PackedMessage::pointer createValidation(Ledger::pointer ledger);

	
};

#endif