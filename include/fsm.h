#ifndef __FSM_H__
#define __FSM_H__

#include <list>
#include <iterator>
#include <exception>
#include <algorithm>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <boost/graph/adjacency_list.hpp>

namespace simple_state_machine {

  class invalid_transition : public std::runtime_error {
  public:
    explicit invalid_transition(const std::string& what)
      : std::runtime_error(what) {}
  };

  class vocabulary_fault : public invalid_transition {
  public:
    explicit vocabulary_fault(const std::string& what)
      : invalid_transition(what) {}
  };
  
  class nondeterministic_fault : public invalid_transition {
  public:
    explicit nondeterministic_fault(const std::string& what)
      : invalid_transition(what) {}
  };
  
  // Event type must be equality comparable
  template <class State, class Event>
  class fsm {

  public:
    typedef boost::adjacency_list<
      boost::listS // edge list selector
      , boost::listS // vertex list selector
      , boost::directedS // bi/directed graph selector
      , State // vertex bundled property
      , Event // edge bundled property
      > transition_map;
    typedef std::list<Event> event_word;

    typedef typename transition_map::edge_descriptor event_descriptor;
    typedef typename transition_map::vertex_descriptor state_descriptor;
    
    typedef std::pair<
      typename transition_map::out_edge_iterator
      , typename transition_map::out_edge_iterator
      > state_events;

    explicit fsm(
		 const state_descriptor& start
		 , const transition_map& map
		 )
      : m_current_state(start) {}
    
    virtual ~fsm() {}

    void process_event(const Event& e);
    void process_word(const event_word& el);

    const State& current() const { return m_transition_map[m_current_state]; }

  protected:
    state_descriptor m_current_state;
    transition_map m_transition_map;
    
    boost::mutex m_mutex;

  private:

    static bool __equal_event_pred(const transition_map& tmap
				   , const event_descriptor& other
				   , const Event& cmp_event) { return tmap[other] == cmp_event; }

    static bool __not_equal_event_pred(const transition_map& tmap
				       , const event_descriptor& other
				       , const Event& cmp_event) { return tmap[other] != cmp_event; }
    
  };

  
  template <class State, class Event>
    void fsm<State, Event>::process_event(const Event& e) {

    boost::mutex::scoped_lock l(m_mutex);

    state_events events(
			out_edges(
				  m_current_state
				  , m_transition_map
				  )
			);
    std::list<event_descriptor> possible_events;
    
    std::remove_copy_if(events.first
			, events.second
			, std::back_inserter(possible_events)
			, boost::bind<bool>(&__not_equal_event_pred
					    , m_transition_map
					    , _1
					    , e));
    
    if(possible_events.empty()) {
      throw vocabulary_fault("event is not defined for current state.");
    }
    else if(possible_events.size() > 1) {
      throw nondeterministic_fault("the state machine is nondeterministic.");
    }
    else {
      m_current_state = boost::target(
				      *possible_events.begin()
				      , m_transition_map
				      );
    }
    
  }
  
  template <class State, class Event>
    void fsm<State, Event>::process_word(const event_word& el) {
    std::for_each(
		  el.begin()
		  , el.end()
		  , boost::bind(
				&fsm::process_event
				, this
				, _1
				)
		  );
  }
  
}

#endif
