#include <boost/test/unit_test.hpp>
#include "fsm.h"

using namespace boost::unit_test;
using namespace std;
using namespace simple_state_machine;

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

test_suite* init_unit_test_suite(int argc, char** argv) {

  return 0;
  
}
