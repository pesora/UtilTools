#if !defined(SerialQue_H)
#define SerialQue_H

#include "Lock.h"
#include "SysEvent.h"

#include <string>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////
// CicularQue
//
// Purpose:		used for circular buffer 

class CircularQue 
{
    long		m_read;					// start position to read from que 
    long		m_write;				// start position to write into que
	long		m_queSz;				// size of que
	long		m_wrapType;				// stores wrap type, did one position get
										// inc to another posisition
    BYTE *		m_que;					// stores recieved commands 

	Lock		m_lock;					// lock for que and pos access

	SysEvent	m_rxEvent;				// recieve event for que
	SysEvent	m_wrapEvent;			// wrap event (shows that a wrap if a about to occur)

protected:


	enum
	{
		idRead,
		idWrite
	};

	enum
	{
		idNoWrap,
		idReadWrap,
		idWriteWrap
	};

	// inc read/write pos based
	// on operation
	bool incPos ( long inc, long op );

	// set read/write pos based
	// on operation
	bool setPos ( long inc, long op );

public:

	CircularQue ();
	~CircularQue ();

	// create/release 
	bool create		( long queSz );
	void release	();

	// get counts
	bool getCount		( long & count, long op );
	bool readCount		( long & count );
	bool writeCount		( long & count );
	long lengthToEnd	( long op );

	// read/write data 
	bool readWithCnt	( BYTE * data, long & length );
	bool read			( BYTE * data, long length );
	bool write			( BYTE * data, long length );

	// read/write data 
	bool read		( string & data );
	bool write		( string & data );

	// get pos 
	long readPos	();
	long writePos	();
	long getPos		( long op );

	// get size of que
	long size		()
	{ return m_queSz; }
};


// get pos 
inline
long CircularQue::readPos ()
{ 
	return m_read; 
}

inline
long CircularQue::writePos ()
{ 
	return m_write; 
}

inline
long CircularQue::getPos ( long op )
{
	if ( op == idRead )
		return m_read;
	else
		return m_write;
}


inline
bool CircularQue::readCount ( long & count )
{
	return getCount(count,idRead);
}

inline
bool CircularQue::writeCount ( long & count )
{
	return getCount(count,idWrite);
}



inline
long CircularQue::lengthToEnd ( long op )
{
	if ( op == idRead )
		return m_queSz - m_read;
	else
		return m_queSz - m_write;
}








class SerialQue :
	public CircularQue
{
public:

    long	m_channel;				// start address of channel 

    long	m_baudRate;			    // baud rate for channel 

    BYTE *	m_sendQue;				// stores command to be sent 
    BYTE *	m_processQue;			// stores command to process 


	SerialQue ();


	void	processCommand	();
	void	readCommand		();
	void	processIt		();

};





#endif