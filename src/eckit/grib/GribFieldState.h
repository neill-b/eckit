// File GribFieldState.h
// Baudouin Raoult - (c) ECMWF Nov 13

#ifndef GribFieldState_H
#define GribFieldState_H

#include "eckit/eckit.h"

namespace eckit {

class GribHandle;
class DataHandle;

class GribFieldState {
public:

// -- Exceptions
	// None

// -- Contructors

    GribFieldState(GribFieldState* next = 0);

// -- Destructor

    virtual ~GribFieldState(); // Change to virtual if base class

// -- Convertors
	// None

// -- Operators
	// None

// -- Methods

    virtual double* returnValues(size_t&) const = 0;
    virtual GribHandle* returnHandle(bool copy) const = 0;
    virtual void write(DataHandle&) const = 0;

    virtual void release() const;

// -- Overridden methods
	// None

// -- Class members
	// None

// -- Class methods
	// None

protected:

// -- Members

    GribFieldState* next_;

// -- Methods
	
    virtual void print(ostream&) const = 0; // Change to virtual if base class



// -- Overridden methods
	// None

// -- Class members
	// None

// -- Class methods
	// None

private:

// No copy allowed

	GribFieldState(const GribFieldState&);
	GribFieldState& operator=(const GribFieldState&);

// -- Members



// -- Methods


// -- Overridden methods
	// None

// -- Class members
	// None

// -- Class methods
	// None

// -- Friends

	friend ostream& operator<<(ostream& s,const GribFieldState& p)
		{ p.print(s); return s; }

    friend class GribField;

};

}

#endif
