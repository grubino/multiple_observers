#include <set>
#include <boost/test/unit_test.hpp>

#include "fsm.h"
#include "observer.h"

using namespace boost::unit_test;
using namespace std;
using namespace simple_state_machine;
using namespace multiple_observer;

BOOST_AUTO_TEST_CASE(check_state) {
  state s_1("test_state_1");
  state s_2("test_state_2");
  state s_3("test_state_3");
  event e_1("test_event_1");
  event e_2("test_event_2");
  
  s_1.insert(make_pair(&e_1, &s_3));
  s_2.insert(make_pair(&e_1, &s_3));
  s_3.insert(make_pair(&e_1, &s_1));
  s_1.insert(make_pair(&e_2, &s_2));
  s_2.insert(make_pair(&e_2, &s_2));
  s_3.insert(make_pair(&e_2, &s_1));

  BOOST_CHECK(s_1[e_1].id() == s_3.id());
  BOOST_CHECK(s_2[e_1].id() == s_3.id());
  BOOST_CHECK(s_3[e_1].id() == s_1.id());
  
  BOOST_CHECK(s_1[e_2].id() == s_2.id());
  BOOST_CHECK(s_2[e_2].id() == s_2.id());
  BOOST_CHECK(s_3[e_2].id() == s_1.id());
}

BOOST_AUTO_TEST_CASE(check_event) {
  BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(check_identifiable) {
  identifiable i_1("identifier");
  BOOST_CHECK(i_1.id() == "identifier");
}

BOOST_AUTO_TEST_CASE(check_fsm) {
  state s_1("test_state_1");
  state s_2("test_state_2");
  state s_3("test_state_3");
  event e_1("test_event_1");
  event e_2("test_event_2");
  
  s_1.insert(make_pair(&e_1, &s_3));
  s_2.insert(make_pair(&e_1, &s_3));
  s_3.insert(make_pair(&e_1, &s_1));
  s_1.insert(make_pair(&e_2, &s_2));
  s_2.insert(make_pair(&e_2, &s_2));
  s_3.insert(make_pair(&e_2, &s_1));

  fsm f(s_1);

  BOOST_CHECK(f.current().id() == s_1.id());

  f.transition(e_1);
  BOOST_CHECK(f.current().id() == s_3.id());

  f.transition(e_1);
  BOOST_CHECK(f.current().id() == s_1.id());

  f.transition(e_2);
  BOOST_CHECK(f.current().id() == s_2.id());

  f.transition(e_2);
  BOOST_CHECK(f.current().id() == s_2.id());
}

BOOST_AUTO_TEST_CASE(check_observer) {

  set<const state*> set_a;
  set<const state*> set_b;
  
  state s_1("test_state_1");
  state s_2("test_state_2");
  state s_3("test_state_3");
  state s_4("test_state_4");
  state s_5("test_state_5");
  state s_6("test_state_6");
  state s_7("test_state_7");
  state s_8("test_state_8");
  state s_9("test_state_9");
  state s_10("test_state_10");
  state s_11("test_state_11");
  state s_12("test_state_12");

  event e_1("test_event_1");
  event e_2("test_event_2");
  event e_3("test_event_3");
  event e_4("test_event_4");
  event e_5("test_event_5");
  event e_6("test_event_6");
  event e_7("test_event_7");
  event e_8("test_event_8");
  
  s_1.insert(make_pair(&e_1, &s_7));
  s_1.insert(make_pair(&e_2, &s_8));
  s_2.insert(make_pair(&e_3, &s_9));
  s_2.insert(make_pair(&e_4, &s_10));
  s_3.insert(make_pair(&e_5, &s_11));
  s_3.insert(make_pair(&e_6, &s_12));
  s_4.insert(make_pair(&e_7, &s_7));
  s_4.insert(make_pair(&e_8, &s_8));
  s_5.insert(make_pair(&e_1, &s_9));
  s_5.insert(make_pair(&e_2, &s_10));
  s_6.insert(make_pair(&e_3, &s_11));
  s_6.insert(make_pair(&e_4, &s_12));
  s_7.insert(make_pair(&e_1, &s_1));
  s_7.insert(make_pair(&e_2, &s_2));
  s_8.insert(make_pair(&e_3, &s_3));
  s_8.insert(make_pair(&e_4, &s_4));
  s_9.insert(make_pair(&e_5, &s_5));
  s_9.insert(make_pair(&e_6, &s_6));
  s_10.insert(make_pair(&e_7, &s_1));
  s_10.insert(make_pair(&e_8, &s_2));
  s_11.insert(make_pair(&e_1, &s_3));
  s_11.insert(make_pair(&e_2, &s_4));
  s_12.insert(make_pair(&e_3, &s_5));
  s_12.insert(make_pair(&e_4, &s_6));

  set_a.insert(&s_1);
  set_a.insert(&s_2);
  set_a.insert(&s_3);
  set_a.insert(&s_4);
  set_a.insert(&s_5);
  set_a.insert(&s_6);
  set_b.insert(&s_7);
  set_b.insert(&s_8);
  set_b.insert(&s_9);
  set_b.insert(&s_10);
  set_b.insert(&s_11);
  set_b.insert(&s_12);
  
  observer obs(s_1, set_a, set_b);
  
  BOOST_CHECK_MESSAGE(
		      obs.current().id() == "s_1",
		      "next: " << obs.current()[e_1].id()
		      );
 
  obs.transition(e_1); // transition to s_7
  BOOST_CHECK_MESSAGE(
		      obs.current().id() == "s_2",
		      "obs.current: " << obs.current().id()
		      );

  obs.transition(e_2); // transition to s_2
  BOOST_CHECK_MESSAGE(obs.current().id() == "s_1", "obs.current: " << obs.current().id());

  obs.transition(e_3); // transition to s_9
  BOOST_CHECK_MESSAGE(obs.current().id() == "s_2", "obs.current: " << obs.current().id());

  obs.transition(e_5); // transition to s_5
  BOOST_CHECK_MESSAGE(obs.current().id() == "s_1", "obs.current: " << obs.current().id());
 	      
}

test_suite* init_unit_test_suite(int argc, char** argv) {

  return 0;
  
}
