#ifndef __IDENTIFIABLE_H__
#define __IDENTIFIABLE_H__

#include <string>

namespace simple_state_machine {
  
  struct identifiable {
  public:
    identifiable(const std::string& id)
    : m_id(id) {}
    ~identifiable() {}
    const std::string& id() const { return m_id; }
  private:
    std::string m_id;
  };

}


#endif
