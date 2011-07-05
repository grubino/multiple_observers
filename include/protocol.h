#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <exception>
#include <string>

namespace protocol {

  class protocol_fault : public std::runtime_error {
  public:
    explicit protocol_fault(const std::string& what)
      : std::runtime_error(what) {}
  };

  class guard_fault : public protocol_fault {
  public:
    explicit guard_fault(const std::string& what)
      : protocol_fault(what) {}
  };

  class sequence_fault : public protocol_fault {
  public:
    explicit sequence_fault(const std::string& what)
      : protocol_fault(what) {}
  };

  class behavior_fault : public protocol_fault {
  public:
    explicit behavior_fault(const std::string& what)
      : protocol_fault(what) {}
  };

  template <class ProtocolState, class ProtocolEvent>
    class protocol : public fsm<ProtocolState, ProtocolEvent>  {};

}

#endif
