#ifndef _DECOMPOSITION_H_
#define _DECOMPOSITION_H_

#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

#include <boost/iterator/permutation_iterator.hpp>

#include "riffle_shuffle.h"

namespace multiple_observers { namespace detail {

template <class RandomAccessIterator>
struct decomposition {

  typedef RandomAccessIterator external_iterator;
  typedef typename std::iterator_traits<RandomAccessIterator>::difference_type index_type;
  typedef typename std::vector<index_type> index_container;
  typedef typename index_container::iterator index_iterator;
  typedef boost::permutation_iterator<external_iterator, index_iterator> permutation_iterator;

  struct generate_indices {

    generate_indices()
    : m_cur(0) {}

    index_type operator()() { return m_cur++; }

  private:
    index_type m_cur;
    
  };

  decomposition(external_iterator __begin, external_iterator __end)
  : m_begin(__begin)
  , m_end(__end) {
    std::generate_n(std::back_inserter(m_indices)
		    , std::distance(m_begin, m_end)
		    , generate_indices());
  }

  template <class OutputIterator>
  void operator()(OutputIterator __left
		  , OutputIterator __right) {

    index_iterator __middle_index = m_indices.begin() + ((m_indices.end() - m_indices.begin()) / 2);
    index_container temp;

    riffle_shuffle(m_indices.begin()
		   , __middle_index
		   , __middle_index
		   , m_indices.end()
		   , back_inserter(temp));

    m_indices.swap(temp);
        
    permutation_iterator __begin(m_begin, m_indices.begin());
    permutation_iterator __end(m_begin, m_indices.end());
    permutation_iterator __middle(__begin + (__end - __begin) / 2);

    std::copy(__begin, __middle, __left);
    std::copy(__middle, __end, __right);
    
  }

  external_iterator m_begin;
  external_iterator m_end;

  index_container m_indices;
  
};

} }
#endif
