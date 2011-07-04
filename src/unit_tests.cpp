#include <set>
#include <vector>
#include <iterator>
#include <algorithm>
#include <boost/test/unit_test.hpp>

#include "fsm.h"

using namespace boost::unit_test;
using namespace std;
using namespace boost;
using namespace simple_state_machine;

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


test_suite* init_unit_test_suite(int argc, char** argv) {

  return 0;
  
}
