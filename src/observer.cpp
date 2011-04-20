#include "observer.h"

namespace multiple_observer {

  using namespace std;
  observer::observer(
		     const ssm::state& start,
		     const std::set<const ssm::state*>& a,
		     const std::set<const ssm::state*>& b
		     )
    : ssm::fsm(start),
      m_state_a(a),
      m_state_b(b),
      m_ref_state_a(REF_STATE_A_ID),
      m_ref_state_b(REF_STATE_B_ID),
      m_unknown_state("") {

    if(a.count(&start))
      m_current_state = &m_ref_state_a;
    else if(b.count(&start))
      m_current_state = &m_ref_state_b;
    else
      m_current_state = &m_unknown_state;

    BOOST_FOREACH(const ssm::state* s, m_state_a) {
      BOOST_FOREACH(const ssm::state::transition t, s->get_transition_map()) {
	if(a.count(t.second))
	  m_ref_state_a.insert(make_pair(t.first, &m_ref_state_a));
	else if(b.count(t.second))
 	  m_ref_state_a.insert(make_pair(t.first, &m_ref_state_b));
	else
	  m_ref_state_a.insert(make_pair(t.first, &m_unknown_state));
      }
    }
    
    BOOST_FOREACH(const ssm::state* s, m_state_b) {
      BOOST_FOREACH(const ssm::state::transition t, s->get_transition_map()) {
	if(a.count(t.second))
	  m_ref_state_b.insert(make_pair(t.first, &m_ref_state_a));
	else if(b.count(t.second))
	  m_ref_state_b.insert(make_pair(t.first, &m_ref_state_b));
	else
	  m_ref_state_b.insert(make_pair(t.first, &m_unknown_state));
      }
    }
    
  }

  void observer::transition(const ssm::event& e) throw(ssm::invalid_transition) {

    boost::mutex::scoped_lock l(m_mutex);
    
    try {
	m_current_state = &(*m_current_state)[e];
    }
    catch (const out_of_range& err) {
      
      m_current_state = &m_unknown_state;
      
      string exception_str;
      exception_str =
	"invalid transition: " +
	m_current_state->id() + " => ??? on event " + e.id();
      throw ssm::invalid_transition(exception_str);
      
    }
    
  }

}
