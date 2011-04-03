#ifndef __FSM_H__
#define __FSM_H__

#include <string>
#include <map>

#include <boost/thread.hpp>

#include "state.h"
#include "event.h"


namespace simple_state_machine {

  class fsm {
  public:
    typedef std::map<event, state> transition_map;
    explicit fsm(const state& start)
      : m_current_state(&start) {}
    ~fsm() {}
    virtual void transition(const event& e) {
      boost::mutex::scoped_lock l(m_mutex);
      m_current_state = &((*m_current_state)[e]);
    }
    virtual const state& current() const { return *m_current_state; }
    virtual ~fsm() {}
  private:
    const state* m_current_state;
    boost::mutex m_mutex;
  };
  
}


#endif
