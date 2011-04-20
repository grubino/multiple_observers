#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include "fsm.h"
#include <set>
#include <exception>

#include <boost/foreach.hpp>

namespace multiple_observer {

  namespace ssm = simple_state_machine;

  const std::string REF_STATE_A_ID("s_1");
  const std::string REF_STATE_B_ID("s_2");

  class observer : public ssm::fsm {
  public:
    observer(
	     const ssm::state& start,
	     const std::set<const ssm::state*>& a,
	     const std::set<const ssm::state*>& b
	     );
    ~observer() {}
    
    void transition(const ssm::event& e) throw(ssm::invalid_transition);
    const ssm::state& current() const { return *m_current_state; }

  private:
    std::set<const ssm::state*> m_state_a;
    std::set<const ssm::state*> m_state_b;
    ssm::state m_ref_state_a;
    ssm::state m_ref_state_b;
    ssm::state m_unknown_state;

  };

}

#endif
