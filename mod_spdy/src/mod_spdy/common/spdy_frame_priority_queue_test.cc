// Copyright 2011 Google Inc.
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

#include "mod_spdy/common/spdy_frame_priority_queue.h"

#include "base/time.h"
#include "net/spdy/spdy_framer.h"
#include "net/spdy/spdy_protocol.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace {

net::SpdyStreamId GetPingId(net::SpdyFrame* frame) {
  if (!frame->is_control_frame() ||
      static_cast<net::SpdyControlFrame*>(frame)->type() != net::PING) {
    ADD_FAILURE() << "Frame is not a PING frame.";
    return 0;
  }
  return static_cast<net::SpdyPingControlFrame*>(frame)->unique_id();
}

void ExpectPop(net::SpdyStreamId expected,
               mod_spdy::SpdyFramePriorityQueue* queue) {
  EXPECT_FALSE(queue->IsEmpty());
  net::SpdyFrame* raw_frame = NULL;
  const bool success = queue->Pop(&raw_frame);
  scoped_ptr<net::SpdyFrame> scoped_frame(raw_frame);
  EXPECT_TRUE(success);
  ASSERT_TRUE(scoped_frame != NULL);
  ASSERT_EQ(expected, GetPingId(scoped_frame.get()));
}

void ExpectEmpty(mod_spdy::SpdyFramePriorityQueue* queue) {
  EXPECT_TRUE(queue->IsEmpty());
  net::SpdyFrame* frame = NULL;
  EXPECT_FALSE(queue->Pop(&frame));
  EXPECT_TRUE(frame == NULL);
}

TEST(SpdyFramePriorityQueueTest, InsertSpdy2) {
  net::SpdyFramer framer(2);
  mod_spdy::SpdyFramePriorityQueue queue;
  ExpectEmpty(&queue);

  EXPECT_EQ(3u, framer.GetLowestPriority());
  EXPECT_EQ(0u, framer.GetHighestPriority());

  queue.Insert(3, framer.CreatePingFrame(4));
  queue.Insert(0, framer.CreatePingFrame(1));
  queue.Insert(3, framer.CreatePingFrame(3));

  ExpectPop(1, &queue);
  ExpectPop(4, &queue);

  queue.Insert(2, framer.CreatePingFrame(2));
  queue.Insert(1, framer.CreatePingFrame(6));
  queue.Insert(1, framer.CreatePingFrame(5));

  ExpectPop(6, &queue);
  ExpectPop(5, &queue);
  ExpectPop(2, &queue);
  ExpectPop(3, &queue);
  ExpectEmpty(&queue);
}

TEST(SpdyFramePriorityQueueTest, InsertSpdy3) {
  net::SpdyFramer framer(3);
  mod_spdy::SpdyFramePriorityQueue queue;
  ExpectEmpty(&queue);

  EXPECT_EQ(7u, framer.GetLowestPriority());
  EXPECT_EQ(0u, framer.GetHighestPriority());

  queue.Insert(7, framer.CreatePingFrame(4));
  queue.Insert(0, framer.CreatePingFrame(1));
  queue.Insert(7, framer.CreatePingFrame(3));

  ExpectPop(1, &queue);
  ExpectPop(4, &queue);

  queue.Insert(6, framer.CreatePingFrame(2));
  queue.Insert(1, framer.CreatePingFrame(6));
  queue.Insert(5, framer.CreatePingFrame(5));

  ExpectPop(6, &queue);
  ExpectPop(5, &queue);
  ExpectPop(2, &queue);
  ExpectPop(3, &queue);
  ExpectEmpty(&queue);
}

TEST(SpdyFramePriorityQueueTest, InsertTopPriority) {
  net::SpdyFramer framer(2);
  mod_spdy::SpdyFramePriorityQueue queue;
  ExpectEmpty(&queue);

  queue.Insert(3, framer.CreatePingFrame(4));
  queue.Insert(mod_spdy::SpdyFramePriorityQueue::kTopPriority,
               framer.CreatePingFrame(2));
  queue.Insert(mod_spdy::SpdyFramePriorityQueue::kTopPriority,
               framer.CreatePingFrame(6));
  queue.Insert(0, framer.CreatePingFrame(1));
  queue.Insert(3, framer.CreatePingFrame(3));

  ExpectPop(2, &queue);
  ExpectPop(6, &queue);
  ExpectPop(1, &queue);
  ExpectPop(4, &queue);

  queue.Insert(mod_spdy::SpdyFramePriorityQueue::kTopPriority,
               framer.CreatePingFrame(5));

  ExpectPop(5, &queue);
  ExpectPop(3, &queue);
  ExpectEmpty(&queue);
}

TEST(SpdyFramePriorityQueueTest, BlockingPop) {
  mod_spdy::SpdyFramePriorityQueue queue;
  net::SpdyFrame* frame;
  ASSERT_FALSE(queue.Pop(&frame));

  const base::TimeDelta time_to_wait = base::TimeDelta::FromMilliseconds(50);
  const base::TimeTicks start = base::TimeTicks::HighResNow();
  ASSERT_FALSE(queue.BlockingPop(time_to_wait, &frame));
  const base::TimeDelta actual_time_waited =
      base::TimeTicks::HighResNow() - start;

  // Check that we waited at least as long as we asked for.
  EXPECT_GE(actual_time_waited, time_to_wait);
  // Check that we didn't wait too much longer than we asked for.
  EXPECT_LT(actual_time_waited.InMillisecondsF(),
            1.1 * time_to_wait.InMillisecondsF());
}

}  // namespace
