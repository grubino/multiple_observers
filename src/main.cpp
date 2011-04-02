#include <iostream>
#include <sstream>
#include <map>
#include <utility>
#include <string>

#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include "fsm.h"

int main(void) {

  using namespace simple_state_machine;
  using namespace std;

  void* _buffer_s = operator new(sizeof(state)*50);
  void* _buffer_e = operator new(sizeof(event)*50);
  
  state* states = static_cast<state*>(_buffer_s);
  event* events = static_cast<event*>(_buffer_e);

  for(unsigned int i = 0; i < 50; ++i) {
    stringstream s;
    s << i;
    string state_name("state_" + s.str());
    string event_name("event_" + s.str());
    cout << "adding state: " << state_name << endl;
    cout << "adding event: " << event_name << endl;
    new(states+i) state(state_name);
    new(events+i) event(event_name);
  }

  for(unsigned int i = 0; i < 50; ++i) {
    states[i].insert(make_pair(events+i, states + ((i+1)%50)));
    cout << "adding transition: " << states[i].id() << " => " << states[i][events[i]].id() << 
      " on event: " << events[i].id() << endl;
    states[i].insert(make_pair(events+(i+1)%50, states + ((i+2)%50)));
    cout << "adding transition: " << states[i].id() << " => " << states[i][events[i]].id() << 
      " on event: " << events[i].id() << endl;
    states[i].insert(make_pair(events+(i+2)%50, states + ((i+3)%50)));
    cout << "adding transition: " << states[i].id() << " => " << states[i][events[i]].id() << 
      " on event: " << events[i].id() << endl;
  }
    
  fsm my_fsm(states[0]);

  cout << "current state: " << my_fsm.current().id() << endl;
  for(unsigned int i = 0; i < 50; ++i) {
    try {
      state::transition_map m(my_fsm.current().get_transition_map());
      BOOST_FOREACH(state::transition t, m) {
	cout << my_fsm.current().id() << " => " << t.second->id() << 
	  " on event " << t.first->id() << endl;
      }

      cout << my_fsm.current().id() << " => " << my_fsm.current()[events[i]].id() << endl;
      my_fsm.transition(events[i]);
    }
    catch (const out_of_range& e) {
      cerr << "there was an error accessing event " << i << endl;
      cerr << "the error was: " << e.what() << endl;
    }
    cout << "current state is: " << my_fsm.current().id() << endl;
  }

  for(unsigned int i = 0; i < 50; ++i) {
    states[49-i].~state();
    events[49-i].~event();
  }

  operator delete [](_buffer_s);
  operator delete [](_buffer_e);
  
  return 0;
  
}
