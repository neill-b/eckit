/*
 * (C) Copyright 1996-2015 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation
 * nor does it submit to any jurisdiction.
 */

/// @file   HistoryCmd.h
/// @author Manuel Fuentes
/// @date   Jan 1997

#ifndef eckit_cmd_HistoryCmd_H
#define eckit_cmd_HistoryCmd_H

#include "eckit/cmd/CmdResource.h"

namespace eckit {

//----------------------------------------------------------------------------------------------------------------------

class HistoryCmd : public CmdResource {
public:
    // -- Contructors

    HistoryCmd();

    // -- Destructor

    ~HistoryCmd();

private:
    // No copy allowed

    HistoryCmd(const HistoryCmd&);
    HistoryCmd& operator=(const HistoryCmd&);

    // -- Overridden methods

    // From CmdResource

    virtual void execute(std::istream&, std::ostream& out, CmdArg&);
    virtual void help(std::ostream&) const;

    virtual Arg usage(const std::string& cmd) const;
};

//----------------------------------------------------------------------------------------------------------------------

}  // namespace eckit

#endif
