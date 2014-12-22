/*
 * (C) Copyright 1996-2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File Double.h
// Baudouin Raoult - ECMWF Dec 97

#ifndef eckit_Double_h
#define eckit_Double_h

#include <stdint.h>

#include "eckit/eckit.h"

//-----------------------------------------------------------------------------

namespace eckit {

//-----------------------------------------------------------------------------

class UUID {

public: // types

	typedef uint8_t value_type;

	typedef uint8_t* iterator;
	typedef uint8_t  const* const_iterator;

public:

	UUID();
	UUID( const std::string& );

	~UUID() {}

public: // methods

	iterator begin() { return data_; } const_iterator begin() const { return data_; }
	iterator end() { return data_ + size(); }
	const_iterator end() const { return data_ + size(); }

	static size_t size() { return 16; }
	static size_t hexSize() { return 32; } // 16 hex digits

	bool isNil() const
	{
		for(size_t i=0; i < size(); ++i)
		{
			if ( data_[i] != 0U )
				return false;
		}
		return true;
	}

	operator std::string() const;

	std::string asString() const;
	void fromString( const std::string& );

protected: // methods

	void print(std::ostream& s) const;

private: // members

	uint8_t data_[16];

	friend std::ostream& operator<<(std::ostream& os,const UUID& u) { u.print(os); return os; }
	friend std::istream& operator>>(std::istream& is,UUID& u);

};

//-----------------------------------------------------------------------------

inline bool operator==(UUID const& lhs, UUID const& rhs)
{
	return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

inline bool operator!=(UUID const& lhs, UUID const& rhs)
{
	return !(lhs == rhs);
}

inline bool operator<(UUID const& lhs, UUID const& rhs)
{
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

inline bool operator>(UUID const& lhs, UUID const& rhs)
{
	return rhs < lhs;
}

inline bool operator<=(UUID const& lhs, UUID const& rhs)
{
	return !(rhs < lhs);
}

inline bool operator>=(UUID const& lhs, UUID const& rhs)
{
	return !(lhs < rhs);
}

//-----------------------------------------------------------------------------

} // namespace eckit

#endif