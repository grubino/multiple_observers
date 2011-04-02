#ifndef __EVENT_H__
#define __EVENT_H__

#include <string>
#include "identifiable.h"

namespace simple_state_machine {
  
  struct event : public identifiable {
  public:
    event(const std::string& id)
      : identifiable(id) {}
    ~event() {}
  };

}  

#endif
