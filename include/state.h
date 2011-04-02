#ifndef __STATE_H__
#define __STATE_H__

#include <utility>
#include <map>
#include <string>
#include "identifiable.h"
#include "event.h"

namespace simple_state_machine {

  struct state : public identifiable {
  public:
    typedef std::map<const event*, const state*> transition_map;
    typedef std::pair<const event*, const state*> transition;
    explicit state(const std::string& id)
      : identifiable(id) {}
    state(const state& s)
      : identifiable(s.id()),
      m_transitions(s.get_transition_map()) {}
    
    ~state() {}
    void insert(const transition& t) { m_transitions.insert(t); }
    const state& operator[](const event& e) const { return *m_transitions.at(&e); }
    const transition_map& get_transition_map() const { return m_transitions; }
  private:
    transition_map m_transitions;
  };
  
}
#endif
