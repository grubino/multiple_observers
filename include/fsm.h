#ifndef __FSM_H__
#define __FSM_H__

#include <string>
#include <map>
#include <exception>

#include <boost/thread.hpp>

#include "state.h"
#include "event.h"


namespace simple_state_machine {

  class invalid_transition : public std::runtime_error {
  public:
    explicit invalid_transition(const std::string& what)
      : std::runtime_error(what) {}
  };
  
  class fsm {

  public:
    typedef std::map<event, state> transition_map;
    
    explicit fsm(const state& start)
      : m_current_state(&start) {}
    
    virtual ~fsm() {}
    
    virtual void transition(const event& e) throw(invalid_transition);
    const state& current() const { return *m_current_state; }
    
  protected:
    const state* m_current_state;
    boost::mutex m_mutex;
    
  };
  
}


#endif
