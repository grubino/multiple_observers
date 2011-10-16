#include <set>
#include <vector>
#include <iterator>
#include <algorithm>
#include <boost/test/unit_test.hpp>
#include <boost/assign.hpp>

#include "fsm.h"
#include "detail/riffle_shuffle.h"
#include "detail/decomposition.h"

using namespace boost::unit_test;
using namespace std;
using namespace boost;
using namespace simple_state_machine;
using namespace multiple_observers;

BOOST_AUTO_TEST_CASE(check_fsm_functionality) {

  fsm<string, string>::transition_map trans_map;

  fsm<string, string>::state_descriptor
    state_0 = add_vertex(string("__state_1"), trans_map);
  fsm<string, string>::state_descriptor
    state_1 = add_vertex(string("__state_2"), trans_map);
  fsm<string, string>::state_descriptor
    state_2 = add_vertex(string("__state_3"), trans_map);
  
  add_edge(state_0, state_1, string("__event_1_2"), trans_map);
  add_edge(state_0, state_2, string("__event_1_3"), trans_map);
  add_edge(state_1, state_2, string("__event_2_3"), trans_map);
  add_edge(state_2, state_0, string("__event_3_1"), trans_map);

  graph_traits<fsm<string, string>::transition_map>::vertex_iterator
    start = vertices(trans_map).first;

  fsm<string, string> my_fsm(*start, trans_map);

  BOOST_CHECK(my_fsm.current() == "__state_1");
  my_fsm.process_event(string("__event_1_2"));
  BOOST_CHECK(my_fsm.current() == "__state_2");
  my_fsm.process_event(string("__event_2_3"));
  BOOST_CHECK(my_fsm.current() == "__state_3");
  my_fsm.process_event(string("__event_3_1"));
  BOOST_CHECK(my_fsm.current() == "__state_1");
  my_fsm.process_event(string("__event_1_3"));
  BOOST_CHECK(my_fsm.current() == "__state_3");

}

BOOST_AUTO_TEST_CASE(check_fsm_faults) {

  
  fsm<string, string>::transition_map trans_map;

  fsm<string, string>::state_descriptor
    state_0 = add_vertex(string("__state_1"), trans_map);
  fsm<string, string>::state_descriptor
    state_1 = add_vertex(string("__state_2"), trans_map);
  fsm<string, string>::state_descriptor
    state_2 = add_vertex(string("__state_3"), trans_map);
  fsm<string, string>::state_descriptor
    state_3 = add_vertex(string("__state_4"), trans_map);
  
  add_edge(state_0, state_1, string("__event_1_2"), trans_map);
  add_edge(state_0, state_2, string("__event_1_3"), trans_map);
  add_edge(state_1, state_2, string("__event_2_3"), trans_map);
  add_edge(state_2, state_0, string("__event_3_1"), trans_map);
  add_edge(state_0, state_3, string("__event_1_4"), trans_map);
  add_edge(state_3, state_0, string("__event__"), trans_map);
  add_edge(state_3, state_1, string("__event__"), trans_map);

  graph_traits<fsm<string, string>::transition_map>::vertex_iterator
    start = vertices(trans_map).first;

  fsm<string, string> my_fsm(*start, trans_map);

  BOOST_CHECK(my_fsm.current() == "__state_1");
  my_fsm.process_event(string("__event_1_2"));
  BOOST_CHECK(my_fsm.current() == "__state_2");
  my_fsm.process_event(string("__event_2_3"));
  BOOST_CHECK(my_fsm.current() == "__state_3");
  my_fsm.process_event(string("__event_3_1"));
  BOOST_CHECK(my_fsm.current() == "__state_1");
  BOOST_CHECK_THROW(my_fsm.process_event(string("__event_2_3")), vocabulary_fault);
  my_fsm.process_event(string("__event_1_4"));
  BOOST_CHECK(my_fsm.current() == "__state_4");
  BOOST_CHECK_THROW(my_fsm.process_event(string("__event__")), nondeterministic_fault);

}

BOOST_AUTO_TEST_CASE(check_fsm_performance) {

  const size_t STATE_COUNT = 1000;
  const size_t EVENT_COUNT = STATE_COUNT * (STATE_COUNT - 1);

  typedef vector<fsm<size_t, size_t>::state_descriptor> state_vector;
  typedef vector<fsm<size_t, size_t>::event_descriptor> event_vector;

  BOOST_TEST_CHECKPOINT("allocating state/event vectors...");
  state_vector states(STATE_COUNT);
  state_vector sources(EVENT_COUNT);
  state_vector targets(EVENT_COUNT);
  event_vector events(EVENT_COUNT);
  
  fsm<size_t, size_t>::transition_map trans_map;

  BOOST_TEST_CHECKPOINT("initializing states...");
  for(size_t i = 0; i < STATE_COUNT; ++i) {
    states[i] = add_vertex(i, trans_map);
  }
  
  BOOST_TEST_CHECKPOINT("initializing event source/target states...");
  for(size_t i = 0; i < EVENT_COUNT; ++i) {
    sources[i] = states[i%STATE_COUNT];
    for(size_t j = 1; j < STATE_COUNT; ++j) {
      targets[i] = states[(i + j)%STATE_COUNT];
    }
  }

  BOOST_TEST_CHECKPOINT("initializing events...");
  for(size_t i = 0; i < EVENT_COUNT; ++i) {
    pair<fsm<size_t, size_t>::event_descriptor, bool> result =
      add_edge(sources[i], targets[i], i, trans_map);
    events[i] = result.first;
  }
    
  fsm<size_t, size_t> my_fsm(states[0], trans_map);

  BOOST_TEST_CHECKPOINT("processing events...");
  my_fsm.process_event(trans_map[events[0]]);
  BOOST_CHECK(trans_map[targets[0]] == my_fsm.current());

  BOOST_TEST_CHECKPOINT("done.");
  
}

BOOST_AUTO_TEST_CASE(riffle_shuffle_test) {

  vector<int> a = assign::list_of(0)(1)(2)(3)(4);
  vector<int> b = assign::list_of(5)(6)(7)(8)(9);
  vector<int> shuffled;

  multiple_observers::detail::riffle_shuffle(a.begin()
					       , a.end()
					       , b.begin()
					       , b.end()
					       , back_inserter(shuffled));

  BOOST_CHECK_MESSAGE(shuffled[0] == 0, "shuffled[0] == " << shuffled[0]);
  BOOST_CHECK_MESSAGE(shuffled[1] == 5, "shuffled[1] == " << shuffled[1]);
  BOOST_CHECK_MESSAGE(shuffled[2] == 1, "shuffled[2] == " << shuffled[2]);
  BOOST_CHECK_MESSAGE(shuffled[3] == 6, "shuffled[3] == " << shuffled[3]);
  BOOST_CHECK_MESSAGE(shuffled[4] == 2, "shuffled[4] == " << shuffled[4]);
  BOOST_CHECK_MESSAGE(shuffled[5] == 7, "shuffled[5] == " << shuffled[5]);
  BOOST_CHECK_MESSAGE(shuffled[6] == 3, "shuffled[6] == " << shuffled[6]);
  BOOST_CHECK_MESSAGE(shuffled[7] == 8, "shuffled[7] == " << shuffled[7]);
  BOOST_CHECK_MESSAGE(shuffled[8] == 4, "shuffled[8] == " << shuffled[8]);
  BOOST_CHECK_MESSAGE(shuffled[9] == 9, "shuffled[9] == " << shuffled[9]);

  a = assign::list_of(0)(1)(2)(3)(4);
  b = assign::list_of(5)(6);
  shuffled.clear();

  multiple_observers::detail::riffle_shuffle(a.begin()
					       , a.end()
					       , b.begin()
					       , b.end()
					       , back_inserter(shuffled));

  BOOST_CHECK_MESSAGE(shuffled[0] == 0, "shuffled[0] == " << shuffled[0]);
  BOOST_CHECK_MESSAGE(shuffled[1] == 5, "shuffled[1] == " << shuffled[1]);
  BOOST_CHECK_MESSAGE(shuffled[2] == 1, "shuffled[2] == " << shuffled[2]);
  BOOST_CHECK_MESSAGE(shuffled[3] == 6, "shuffled[3] == " << shuffled[3]);
  BOOST_CHECK_MESSAGE(shuffled[4] == 2, "shuffled[4] == " << shuffled[4]);
  BOOST_CHECK_MESSAGE(shuffled[5] == 3, "shuffled[5] == " << shuffled[5]);
  BOOST_CHECK_MESSAGE(shuffled[6] == 4, "shuffled[6] == " << shuffled[6]);
  
}

BOOST_AUTO_TEST_CASE(decomposition_test) {

  vector<int> input = assign::list_of(0)(1)(2)(3)(4)(5)(6)(7)(8)(9);
  vector<int> left;
  vector<int> right;

  using multiple_observers::detail::decomposition;

  decomposition<vector<int>::iterator> d(input.begin(), input.end());
  
  d(back_inserter(left), back_inserter(right));
  
  BOOST_CHECK_MESSAGE(left[0] == 0, "left[0] == " << left[0]);
  BOOST_CHECK_MESSAGE(left[1] == 5, "left[1] == " << left[1]);
  BOOST_CHECK_MESSAGE(left[2] == 1, "left[2] == " << left[2]);
  BOOST_CHECK_MESSAGE(left[3] == 6, "left[3] == " << left[3]);
  BOOST_CHECK_MESSAGE(left[4] == 2, "left[4] == " << left[4]);

  BOOST_CHECK_MESSAGE(right[0] == 7, "right[0] == " << right[0]);
  BOOST_CHECK_MESSAGE(right[1] == 3, "right[1] == " << right[1]);
  BOOST_CHECK_MESSAGE(right[2] == 8, "right[2] == " << right[2]);
  BOOST_CHECK_MESSAGE(right[3] == 4, "right[3] == " << right[3]);
  BOOST_CHECK_MESSAGE(right[4] == 9, "right[4] == " << right[4]);

  d(left.begin(), right.begin());
  
  BOOST_CHECK_MESSAGE(left[0] == 0, "left[0] == " << left[0]);
  BOOST_CHECK_MESSAGE(left[1] == 7, "left[1] == " << left[1]);
  BOOST_CHECK_MESSAGE(left[2] == 5, "left[2] == " << left[2]);
  BOOST_CHECK_MESSAGE(left[3] == 3, "left[3] == " << left[3]);
  BOOST_CHECK_MESSAGE(left[4] == 1, "left[4] == " << left[4]);

  BOOST_CHECK_MESSAGE(right[0] == 8, "right[0] == " << right[0]);
  BOOST_CHECK_MESSAGE(right[1] == 6, "right[1] == " << right[1]);
  BOOST_CHECK_MESSAGE(right[2] == 4, "right[2] == " << right[2]);
  BOOST_CHECK_MESSAGE(right[3] == 2, "right[3] == " << right[3]);
  BOOST_CHECK_MESSAGE(right[4] == 9, "right[4] == " << right[4]);

  d(left.begin(), right.begin());
  
  BOOST_CHECK_MESSAGE(left[0] == 0, "left[0] == " << left[0]);
  BOOST_CHECK_MESSAGE(left[1] == 8, "left[1] == " << left[1]);
  BOOST_CHECK_MESSAGE(left[2] == 7, "left[2] == " << left[2]);
  BOOST_CHECK_MESSAGE(left[3] == 6, "left[3] == " << left[3]);
  BOOST_CHECK_MESSAGE(left[4] == 5, "left[4] == " << left[4]);

  BOOST_CHECK_MESSAGE(right[0] == 4, "right[0] == " << right[0]);
  BOOST_CHECK_MESSAGE(right[1] == 3, "right[1] == " << right[1]);
  BOOST_CHECK_MESSAGE(right[2] == 2, "right[2] == " << right[2]);
  BOOST_CHECK_MESSAGE(right[3] == 1, "right[3] == " << right[3]);
  BOOST_CHECK_MESSAGE(right[4] == 9, "right[4] == " << right[4]);

  d(left.begin(), right.begin());
  
  BOOST_CHECK_MESSAGE(left[0] == 0, "left[0] == " << left[0]);
  BOOST_CHECK_MESSAGE(left[1] == 4, "left[1] == " << left[1]);
  BOOST_CHECK_MESSAGE(left[2] == 8, "left[2] == " << left[2]);
  BOOST_CHECK_MESSAGE(left[3] == 3, "left[3] == " << left[3]);
  BOOST_CHECK_MESSAGE(left[4] == 7, "left[4] == " << left[4]);

  BOOST_CHECK_MESSAGE(right[0] == 2, "right[0] == " << right[0]);
  BOOST_CHECK_MESSAGE(right[1] == 6, "right[1] == " << right[1]);
  BOOST_CHECK_MESSAGE(right[2] == 1, "right[2] == " << right[2]);
  BOOST_CHECK_MESSAGE(right[3] == 5, "right[3] == " << right[3]);
  BOOST_CHECK_MESSAGE(right[4] == 9, "right[4] == " << right[4]);

  d(left.begin(), right.begin());
  
  BOOST_CHECK_MESSAGE(left[0] == 0, "left[0] == " << left[0]);
  BOOST_CHECK_MESSAGE(left[1] == 2, "left[1] == " << left[1]);
  BOOST_CHECK_MESSAGE(left[2] == 4, "left[2] == " << left[2]);
  BOOST_CHECK_MESSAGE(left[3] == 6, "left[3] == " << left[3]);
  BOOST_CHECK_MESSAGE(left[4] == 8, "left[4] == " << left[4]);

  BOOST_CHECK_MESSAGE(right[0] == 1, "right[0] == " << right[0]);
  BOOST_CHECK_MESSAGE(right[1] == 3, "right[1] == " << right[1]);
  BOOST_CHECK_MESSAGE(right[2] == 5, "right[2] == " << right[2]);
  BOOST_CHECK_MESSAGE(right[3] == 7, "right[3] == " << right[3]);
  BOOST_CHECK_MESSAGE(right[4] == 9, "right[4] == " << right[4]);

  d(left.begin(), right.begin());
  
  BOOST_CHECK_MESSAGE(left[0] == 0, "left[0] == " << left[0]);
  BOOST_CHECK_MESSAGE(left[1] == 1, "left[1] == " << left[1]);
  BOOST_CHECK_MESSAGE(left[2] == 2, "left[2] == " << left[2]);
  BOOST_CHECK_MESSAGE(left[3] == 3, "left[3] == " << left[3]);
  BOOST_CHECK_MESSAGE(left[4] == 4, "left[4] == " << left[4]);

  BOOST_CHECK_MESSAGE(right[0] == 5, "right[0] == " << right[0]);
  BOOST_CHECK_MESSAGE(right[1] == 6, "right[1] == " << right[1]);
  BOOST_CHECK_MESSAGE(right[2] == 7, "right[2] == " << right[2]);
  BOOST_CHECK_MESSAGE(right[3] == 8, "right[3] == " << right[3]);
  BOOST_CHECK_MESSAGE(right[4] == 9, "right[4] == " << right[4]);

}

test_suite* init_unit_test_suite(int argc, char** argv) {

  return 0;
  
}
