#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include "fsm.h"
#include <set>

namespace multiple_observer {

  namespace ssm = simple_state_machine;
  
  class observer : public ssm::fsm {
  public:
    observer(
	     const ssm::state& start,
	     const std::set<ssm::state*>& a,
	     const std::set<ssm::state*>& b
	     )
      : ssm::fsm(&start), m_state_a(a), m_state_b(b) {}

  private:
    std::set<const ssm::state*> m_state_a;
    std::set<const ssm::state*> m_state_b;
    
  };

}

#endif
