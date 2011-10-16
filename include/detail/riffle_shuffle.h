#ifndef _RIFFLE_SHUFFLE_H_
#define _RIFFLE_SHUFFLE_H_

namespace multiple_observers { namespace detail {
  
template <class InputIterator, class OutputIterator>
void riffle_shuffle(InputIterator __left_begin
		    , InputIterator __left_end
		    , InputIterator __right_begin
		    , InputIterator __right_end
		    , OutputIterator __out) {

  for(bool b = false
	; __left_begin != __left_end
	or __right_begin != __right_end
	; b = !b) {
    if(b and __right_begin != __right_end) {
      *__out = *__right_begin;
      ++__right_begin; ++__out;
    }
    else {
      *__out = *__left_begin;
      ++__left_begin; ++__out;
    }
  }
  
}

} }

#endif
