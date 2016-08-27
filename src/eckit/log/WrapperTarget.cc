/*
 * (C) Copyright 1996-2016 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/exception/Exceptions.h"

#include "eckit/log/WrapperTarget.h"

namespace eckit {

//----------------------------------------------------------------------------------------------------------------------

WrapperTarget::WrapperTarget(LogTarget* target):
    target_(target),
    prefix_(true) {
    ASSERT(target);
}

WrapperTarget::~WrapperTarget() {
    delete target_;
}

void WrapperTarget::write(const char* start, const char* end) {

    const char *begin = start;

    while (start != end) {
        if (*start == '\n') {
            target_->write(begin, start);
            writeSuffix();
            target_->write(start, start + 1);
            prefix_ = true;
            begin = start + 1;
        }
        else {
            if (prefix_) {
                writePrefix();
                prefix_ = false;
            }
        }
        start++;
    }

}

void WrapperTarget::flush() {
}

//----------------------------------------------------------------------------------------------------------------------

} // namespace eckit
