/*
 * (C) Copyright 1996-2015 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/types/Types.h"
#include "eckit/exception/Exceptions.h"
#include "eckit/parser/StringTools.h"
#include "experimental/eckit/ecml/parser/Request.h"

#include "experimental/eckit/ecml/core/ExecutionContext.h"
#include "experimental/eckit/ecml/core/Environment.h"
#include "experimental/eckit/ecml/core/Interpreter.h"

#include "experimental/eckit/ecml/ast/FunctionDefinition.h"
#include "experimental/eckit/ecml/ast/Closure.h"
#include "experimental/eckit/ecml/prelude/REPLHandler.h"

using namespace std;

namespace eckit {

REPLHandler::REPLHandler(const string& name)
: SpecialFormHandler(name)
{}

Request REPLHandler::handle(const Request, ExecutionContext& context)
{
    repl(context);

    return new Cell("_list", "", 0, 0);
}


string REPLHandler::historyFile()
{
    return string (getenv("HOME")) + "/.ecml_history";
}

void REPLHandler::readHistory()
{
}


void REPLHandler::writeHistory()
{
}

string REPLHandler::readLine()
{
    string r;
    cout << "ecml> ";
    getline (cin, r);
    return r;
}

void REPLHandler::repl(ExecutionContext& context)
{
    readHistory();
    string cmd; 
    while (true)
    {
        cmd += readLine();

        if (cin.eof()
            || cmd == "quit"
            || cmd == "bye")
        {
            writeHistory();
            cout << "Bye." << endl;
            break;
        }

        if (! cmd.size()) continue;

        if (cmd.rfind("\\") == cmd.size() - 1)
        {
            cmd.erase(cmd.size() - 1);
            cmd += '\n';
            continue;
        }

        try {
            Values r (context.execute(cmd));
            cout << " => " << r << endl;
            if (showResultGraph(context))
                r->graph();
            delete r;
        } catch (Exception e) {
            // error message is already printed by Exception ctr
            //cout << "*** error: " << e.what() << endl;
        }
        cmd = "";
    }
}

bool REPLHandler::showResultGraph(ExecutionContext& context)
{
    if (! context.environment().lookupNoThrow("show_dot"))
        return false;
    else
    {
        vector<string> vs (context.environment().lookupList("show_dot", context));
        return vs.size() == 1 && vs[0] == "true";
    }
}

} // namespace eckit
