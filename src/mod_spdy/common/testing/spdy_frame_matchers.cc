// Copyright 2012 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "mod_spdy/common/testing/spdy_frame_matchers.h"

#include <iostream>
#include <string>

#include "base/stringprintf.h"
#include "net/spdy/spdy_framer.h"
#include "net/spdy/spdy_protocol.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace {

std::string HeadersString(const net::SpdyHeaderBlock& headers) {
  std::string ret = "{ ";
  bool comma = false;
  for (net::SpdyHeaderBlock::const_iterator iter = headers.begin();
       iter != headers.end(); ++iter) {
    if (comma) {
      ret += ", ";
    }
    ret += "'" + iter->first + "': '" + iter->second + "'";
    comma = true;
  }
  ret += " }";
  return ret;
}

}  // namespace

namespace mod_spdy {

namespace testing {

bool IsControlFrameOfTypeMatcher::MatchAndExplain(
    const net::SpdyFrame& frame,
    ::testing::MatchResultListener* listener) const {
  if (!frame.is_control_frame()) {
    *listener << "is a data frame";
    return false;
  }
  const net::SpdyControlFrame* ctrl_frame =
      static_cast<const net::SpdyControlFrame*>(&frame);
  if (ctrl_frame->type() != type_) {
    *listener << "is a " << net::SpdyFramer::ControlTypeToString(
        ctrl_frame->type()) << " frame";
    return false;
  }
  return true;
}

void IsControlFrameOfTypeMatcher::DescribeTo(std::ostream* out) const {
  *out << "is a " << net::SpdyFramer::ControlTypeToString(type_) << " frame";
}

void IsControlFrameOfTypeMatcher::DescribeNegationTo(std::ostream* out) const {
  *out << "isn't a " << net::SpdyFramer::ControlTypeToString(type_)
       << " frame";
}

bool IsDataFrameMatcher::MatchAndExplain(
    const net::SpdyFrame& frame,
    ::testing::MatchResultListener* listener) const {
  if (frame.is_control_frame()) {
    *listener << "is a " << net::SpdyFramer::ControlTypeToString(
        static_cast<const net::SpdyControlFrame*>(&frame)->type())
              << " frame";
    return false;
  }
  return true;
}

void IsDataFrameMatcher::DescribeTo(std::ostream* out) const {
  *out << "is a data frame";
}

void IsDataFrameMatcher::DescribeNegationTo(std::ostream* out) const {
  *out << "isn't a data frame";
}

bool IsDataFrameWithMatcher::MatchAndExplain(
    const net::SpdyFrame& frame,
    ::testing::MatchResultListener* listener) const {
  if (frame.is_control_frame()) {
    *listener << "is a " << net::SpdyFramer::ControlTypeToString(
        static_cast<const net::SpdyControlFrame*>(&frame)->type())
              << " frame";
    return false;
  }
  const base::StringPiece actual_payload(
      static_cast<const net::SpdyDataFrame*>(&frame)->payload(),
      frame.length());
  if (actual_payload != payload_) {
    *listener << "is a data frame with payload \"" << actual_payload << "\"";
    return false;
  }
  return true;
}

void IsDataFrameWithMatcher::DescribeTo(std::ostream* out) const {
  *out << "is a data frame with payload \"" << payload_ << "\"";
}

void IsDataFrameWithMatcher::DescribeNegationTo(std::ostream* out) const {
  *out << "isn't a data frame with payload \"" << payload_ << "\"";
}

bool IsGoAwayMatcher::MatchAndExplain(
    const net::SpdyFrame& frame,
    ::testing::MatchResultListener* listener) const {
  if (!frame.is_control_frame()) {
    *listener << "is a data frame";
    return false;
  }
  const net::SpdyControlFrame* ctrl_frame =
      static_cast<const net::SpdyControlFrame*>(&frame);
  if (ctrl_frame->type() != net::GOAWAY) {
    *listener << "is a " << net::SpdyFramer::ControlTypeToString(
        ctrl_frame->type()) << " frame";
    return false;
  }
  // The GOAWAY status field only exists for SPDY v3 and later, so for earlier
  // versions just skip this check.
  if (ctrl_frame->version() >= 3) {
    const net::SpdyGoAwayControlFrame* go_away_frame =
        static_cast<const net::SpdyGoAwayControlFrame*>(ctrl_frame);
    if (go_away_frame->status() != status_) {
      *listener << "is a GOAWAY frame with status " << go_away_frame->status();
      return false;
    }
  }
  return true;
}

void IsGoAwayMatcher::DescribeTo(std::ostream* out) const {
  *out << "is a GOAWAY frame with status " << status_;
}

void IsGoAwayMatcher::DescribeNegationTo(std::ostream* out) const {
  *out << "isn't a GOAWAY frame with status " << status_;
}

bool IsRstStreamMatcher::MatchAndExplain(
    const net::SpdyFrame& frame,
    ::testing::MatchResultListener* listener) const {
  if (!frame.is_control_frame()) {
    *listener << "is a data frame";
    return false;
  }
  const net::SpdyControlFrame* ctrl_frame =
      static_cast<const net::SpdyControlFrame*>(&frame);
  if (ctrl_frame->type() != net::RST_STREAM) {
    *listener << "is a " << net::SpdyFramer::ControlTypeToString(
        ctrl_frame->type()) << " frame";
    return false;
  }
  const net::SpdyRstStreamControlFrame* rst_stream_frame =
      static_cast<const net::SpdyRstStreamControlFrame*>(ctrl_frame);
  if (rst_stream_frame->status() != status_) {
    *listener << "is a RST_STREAM frame with status "
              << rst_stream_frame->status();
    return false;
  }
  return true;
}

void IsRstStreamMatcher::DescribeTo(std::ostream* out) const {
  *out << "is a RST_STREAM frame with status " << status_;
}

void IsRstStreamMatcher::DescribeNegationTo(std::ostream* out) const {
  *out << "isn't a RST_STREAM frame with status " << status_;
}

bool IsWindowUpdateMatcher::MatchAndExplain(
    const net::SpdyFrame& frame,
    ::testing::MatchResultListener* listener) const {
  if (!frame.is_control_frame()) {
    *listener << "is a data frame";
    return false;
  }
  const net::SpdyControlFrame* ctrl_frame =
      static_cast<const net::SpdyControlFrame*>(&frame);
  if (ctrl_frame->type() != net::WINDOW_UPDATE) {
    *listener << "is a " << net::SpdyFramer::ControlTypeToString(
        ctrl_frame->type()) << " frame";
    return false;
  }
  const net::SpdyWindowUpdateControlFrame* window_update_frame =
      static_cast<const net::SpdyWindowUpdateControlFrame*>(ctrl_frame);
  if (window_update_frame->delta_window_size() != delta_) {
    *listener << "is a WINDOW_UPDATE frame with delta="
              << window_update_frame->delta_window_size();
    return false;
  }
  return true;
}

void IsWindowUpdateMatcher::DescribeTo(std::ostream* out) const {
  *out << "is a WINDOW_UPDATE frame with delta=" << delta_;
}

void IsWindowUpdateMatcher::DescribeNegationTo(std::ostream* out) const {
  *out << "isn't a WINDOW_UPDATE frame with delta=" << delta_;
}

bool FlagFinIsMatcher::MatchAndExplain(
    const net::SpdyFrame& frame,
    ::testing::MatchResultListener* listener) const {
  const bool fin = frame.is_control_frame() ?
      (frame.flags() & net::CONTROL_FLAG_FIN) :
      (frame.flags() & net::DATA_FLAG_FIN);
  if (fin != fin_) {
    *listener << (fin ? "has FLAG_FIN set" : "doesn't have FLAG_FIN set");
    return false;
  }
  return true;
}

void FlagFinIsMatcher::DescribeTo(std::ostream* out) const {
  *out << (fin_ ? "has FLAG_FIN set" : "doesn't have FLAG_FIN set");
}

void FlagFinIsMatcher::DescribeNegationTo(std::ostream* out) const {
  *out << (fin_ ? "doesn't have FLAG_FIN set" : "has FLAG_FIN set");
}

bool FlagUnidirectionalIsMatcher::MatchAndExplain(
    const net::SpdyFrame& frame,
    ::testing::MatchResultListener* listener) const {
  if (!frame.is_control_frame()) {
    *listener << "is a data frame";
    return false;
  }
  const bool unidirectional =
      (frame.flags() & net::CONTROL_FLAG_UNIDIRECTIONAL);
  if (unidirectional != unidirectional_) {
    *listener << (unidirectional ? "has FLAG_UNIDIRECTIONAL set" :
                  "doesn't have FLAG_UNIDIRECTIONAL set");
    return false;
  }
  return true;
}

void FlagUnidirectionalIsMatcher::DescribeTo(std::ostream* out) const {
  *out << (unidirectional_ ? "has FLAG_UNIDIRECTIONAL set" :
           "doesn't have FLAG_UNIDIRECTIONAL set");
}

void FlagUnidirectionalIsMatcher::DescribeNegationTo(std::ostream* out) const {
  *out << (unidirectional_ ? "doesn't have FLAG_UNIDIRECTIONAL set" :
           "has FLAG_UNIDIRECTIONAL set");
}

bool StreamIdIsMatcher::MatchAndExplain(
    const net::SpdyFrame& frame,
    ::testing::MatchResultListener* listener) const {
  net::SpdyStreamId id;
  if (frame.is_control_frame()) {
    net::SpdyControlType type =
        static_cast<const net::SpdyControlFrame*>(&frame)->type();
    switch (type) {
      case net::SYN_STREAM:
        id = static_cast<const net::SpdySynStreamControlFrame*>(
            &frame)->stream_id();
        break;
      case net::SYN_REPLY:
        id = static_cast<const net::SpdySynReplyControlFrame*>(
            &frame)->stream_id();
        break;
      case net::RST_STREAM:
        id = static_cast<const net::SpdyRstStreamControlFrame*>(
            &frame)->stream_id();
        break;
      case net::HEADERS:
        id = static_cast<const net::SpdyHeadersControlFrame*>(
            &frame)->stream_id();
        break;
      case net::WINDOW_UPDATE:
        id = static_cast<const net::SpdyWindowUpdateControlFrame*>(
            &frame)->stream_id();
        break;
      default:
        *listener << "is a " << net::SpdyFramer::ControlTypeToString(type)
                  << " frame";
        return false;
    }
  } else {
    id = static_cast<const net::SpdyDataFrame*>(&frame)->stream_id();
  }
  if (id != stream_id_) {
    *listener << "has stream ID " << id;
    return false;
  }
  return true;
}

void StreamIdIsMatcher::DescribeTo(std::ostream* out) const {
  *out << "has stream ID " << stream_id_;
}

void StreamIdIsMatcher::DescribeNegationTo(std::ostream* out) const {
  *out << "doesn't have stream ID " << stream_id_;
}

bool AssociatedStreamIdIsMatcher::MatchAndExplain(
    const net::SpdyFrame& frame,
    ::testing::MatchResultListener* listener) const {
  if (!frame.is_control_frame()) {
    *listener << "is a data frame";
    return false;
  }
  const net::SpdyControlType type =
      static_cast<const net::SpdyControlFrame*>(&frame)->type();
  if (type != net::SYN_STREAM) {
    *listener << "is a " << net::SpdyFramer::ControlTypeToString(type)
              << " frame";
    return false;
  }
  const net::SpdyStreamId id =
      static_cast<const net::SpdySynStreamControlFrame*>(
          &frame)->associated_stream_id();
  if (id != associated_stream_id_) {
    *listener << "has associated stream ID " << id;
    return false;
  }
  return true;
}

void AssociatedStreamIdIsMatcher::DescribeTo(std::ostream* out) const {
  *out << "has associated stream ID " << associated_stream_id_;
}

void AssociatedStreamIdIsMatcher::DescribeNegationTo(std::ostream* out) const {
  *out << "doesn't have associated stream ID " << associated_stream_id_;
}

bool PriorityIsMatcher::MatchAndExplain(
    const net::SpdyFrame& frame,
    ::testing::MatchResultListener* listener) const {
  if (!frame.is_control_frame()) {
    *listener << "is a data frame";
    return false;
  }
  const net::SpdyControlType type =
      static_cast<const net::SpdyControlFrame*>(&frame)->type();
  if (type != net::SYN_STREAM) {
    *listener << "is a " << net::SpdyFramer::ControlTypeToString(type)
              << " frame";
    return false;
  }
  const net::SpdyPriority pri =
      static_cast<const net::SpdySynStreamControlFrame*>(
          &frame)->priority();
  if (pri != priority_) {
    *listener << "has priority " << pri;
    return false;
  }
  return true;
}

void PriorityIsMatcher::DescribeTo(std::ostream* out) const {
  *out << "has priority " << priority_;
}

void PriorityIsMatcher::DescribeNegationTo(std::ostream* out) const {
  *out << "doesn't have priority " << priority_;
}

bool UncompressedHeadersAreMatcher::MatchAndExplain(
    const net::SpdyFrame& frame,
    ::testing::MatchResultListener* listener) const {
  if (!frame.is_control_frame()) {
    *listener << "is a data frame";
    return false;
  }
  const net::SpdyControlFrame* ctrl_frame =
      static_cast<const net::SpdyControlFrame*>(&frame);
  const char* header_block;
  size_t header_block_len;
  net::SpdyControlType type = ctrl_frame->type();
  switch (type) {
    case net::SYN_STREAM:
      header_block = static_cast<const net::SpdySynStreamControlFrame*>(
          ctrl_frame)->header_block();
      header_block_len = static_cast<const net::SpdySynStreamControlFrame*>(
          ctrl_frame)->header_block_len();
      break;
    case net::SYN_REPLY:
      header_block = static_cast<const net::SpdySynReplyControlFrame*>(
          ctrl_frame)->header_block();
      header_block_len = static_cast<const net::SpdySynReplyControlFrame*>(
          ctrl_frame)->header_block_len();
      break;
    case net::HEADERS:
      header_block = static_cast<const net::SpdyHeadersControlFrame*>(
          ctrl_frame)->header_block();
      header_block_len = static_cast<const net::SpdyHeadersControlFrame*>(
          ctrl_frame)->header_block_len();
      break;
    default:
      *listener << "is a " << net::SpdyFramer::ControlTypeToString(type)
                << " frame";
      return false;
  }
  net::SpdyFramer framer(ctrl_frame->version());
  net::SpdyHeaderBlock actual_headers;
  if (!framer.ParseHeaderBlockInBuffer(header_block, header_block_len,
                                       &actual_headers)) {
    *listener << "headers data is compressed/corrupted";
    return false;
  }
  if (actual_headers != headers_) {
    *listener << "has uncompressed headers " << HeadersString(actual_headers);
    return false;
  }
  return true;
}

void UncompressedHeadersAreMatcher::DescribeTo(std::ostream* out) const {
  *out << "has uncompressed headers " << HeadersString(headers_);
}

void UncompressedHeadersAreMatcher::DescribeNegationTo(
    std::ostream* out) const {
  *out << "doesn't have uncompressed headers " << HeadersString(headers_);
}

}  // namespace testing

}  // namespace mod_spdy
