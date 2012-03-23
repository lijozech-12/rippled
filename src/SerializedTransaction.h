#ifndef __SERIALIZEDTRANSACTION__
#define __SERIALIZEDTRANSACTION__

#include <vector>

#include "uint256.h"
#include "SerializedObject.h"
#include "TransactionFormats.h"

class SerializedTransaction : public STObject
{
protected:
	TransactionType type;
	STVariableLength mSignature;
	STObject mMiddleTxn, mInnerTxn;
	TransactionFormat* mFormat;

public:
	SerializedTransaction(SerializerIterator&, int length);
	SerializedTransaction(TransactionType type);

	// STObject functions
	int getLength() const;
	SerializedTypeID getType() const { return STI_TRANSACTION; }
	SerializedTransaction* duplicate() const { return new SerializedTransaction(*this); }
	std::string getFullText() const;
	std::string getText() const;
	void add(Serializer& s) const { getTransaction(s, true); }

	// outer transaction functions / signature functions
	std::vector<unsigned char> getSignature() const;
	void setSignature(const std::vector<unsigned char>& s);
	uint256 getSigningHash() const;
		
	// middle transaction functions
	uint32 getVersion() const;
	void setVersion(uint32);
	TransactionType getTxnType() const { return type; }
	uint64 getTransactionFee() const;
	void setTransactionFee(uint64);

	// inner transaction functions
	uint16 getFlags() const;
	void setFlag(int v);
	void clearFlag(int v);
	bool isFlag(int v);
	
	uint32 getSequence() const;
	void setSequence(uint32);

	// inner transaction field functions
	int getITFieldIndex(const char *) const;
	int getITFieldCount() const;
	bool getITFieldPresent(int index) const;
	const SerializedType& peekITField(int index);
	SerializedType& getITField(int index);
	void makeITFieldPresent(int index);

	// whole transaction functions
	int getTransaction(Serializer& s, bool include_length) const;
	uint256 getTransactionID() const;
};

#endif
