#include <iostream>
#include <sstream>
#include <utility>
#include <string>

#include <boost/foreach.hpp>

#include <boost/graph/adjacency_list_io.hpp>

#include "fsm.h"

int main(void) {

  using namespace simple_state_machine;
  using namespace boost;
  using namespace std;

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

  cout << my_fsm.current() << endl;
  my_fsm.process_event(string("__event_1_2"));

  cout << my_fsm.current() << endl;
  my_fsm.process_event(string("__event_2_3"));

  cout << my_fsm.current() << endl;
  my_fsm.process_event(string("__event_3_1"));

  cout << my_fsm.current() << endl;
  my_fsm.process_event(string("__event_1_3"));

  cout << my_fsm.current() << endl;

  return 0;
  
}
