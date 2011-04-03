#include "fsm.h"

namespace simple_state_machine {

  using namespace std;

  void fsm::transition(const event& e) throw(invalid_transition) {
    
    boost::mutex::scoped_lock l(m_mutex);
    
    try {
      
      m_current_state = &((*m_current_state)[e]);
      
    }
    catch (const out_of_range& e) {
      
      throw invalid_transition(e.what());
      
    }
    
  }

}
