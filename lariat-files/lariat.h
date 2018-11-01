///////////////////////////////////////////////////////////////////////////////
#ifndef LARIAT_H
#define LARIAT_H
///////////////////////////////////////////////////////////////////////////////

#include <string>     // error strings
#include <utility>    // error strings
//#include <cstring>     // memcpy

class LariatException : public std::exception {
private:
  int m_ErrCode;
  std::string m_Description;

public:
  LariatException(const int ErrCode, std::string Description) :
    m_ErrCode(ErrCode), m_Description(std::move(Description)) {}

  virtual int code() const
  {
    return m_ErrCode;
  }

  virtual const char* what() const noexcept
  {
    return m_Description.c_str();
  }

  virtual ~LariatException() noexcept = default;

  enum LARIAT_EXCEPTION { E_NO_MEMORY, E_BAD_INDEX, E_DATA_ERROR };
};

// forward declaration for 1-1 operator<< 
template<typename T, int Size>
class Lariat;

template<typename T, int Size>
std::ostream& operator<< (std::ostream& os, Lariat<T, Size> const& rhs);

template <typename T, int Size>
class Lariat
{
public:
  Lariat();                  // default constructor                        
  Lariat(Lariat const& rhs); // copy constructor
  ~Lariat(); // destructor
  // more ctor(s) and assignment(s)

  // inserts
  void insert(int index, T const& value);
  void push_back(T const& value);
  void push_front(T const& value);

  // deletes
  void erase(int index);
  void pop_back();
  void pop_front();

  //access
  T&       operator[](int index);       // for l-values
  T const& operator[](int index) const; // for r-values
  T&       first();
  T const& first() const;
  T&       last();
  T const& last() const;

  // returns index, size (one past last) if not found
  unsigned find(T const& value) const;

  friend std::ostream& operator<< <T, Size>(
    std::ostream& os, Lariat<T, Size> const& rhs
    );

  // and some more
  size_t size() const;   // total number of items (not nodes)
  void clear();          // make it empty

  void compact();             // push data in front reusing empty positions and delete remaining nodes

private:
  // DO NOT modify provided code
  struct LNode
  {
    LNode* next = nullptr;
    LNode* prev = nullptr;
    int    count = 0;         // number of items currently in the node
    T values[Size];
  };
  // DO NOT modify provided code
  LNode* head_;           // points to the first node
  LNode* tail_;           // points to the last node
  int size_;              // the number of items (not nodes) in the list
  mutable int nodecount_; // the number of nodes in the list
  int asize_;             // the size of the array within the nodes

  // todo
  //+ Recommended Helper Functions
  LNode* allocate();
  void add_value(LNode* node, size_t ind, T const& val);
  void remove_value(LNode* node/*, size_t ind, T const& val*/);
  //  - split
  LNode* split(LNode* node);
  //  - findElement
  //  - shiftUp
  //  - shiftDown
};

//#include "lariat.cpp"

#endif // LARIAT_H


//#include <iostream>
//#include <iomanip>

#if 1
template <typename T, int Size>
std::ostream& operator<<(std::ostream &os, Lariat<T, Size> const & rhs)
{
  typename Lariat<T, Size>::LNode * current = rhs.head_;
  int index = 0;
  while (current) {
    os << "Node starting (count " << current->count << ")\n";
    for (int local_index = 0; local_index < current->count; ++local_index) {
      os << index << " -> " << current->values[local_index] << std::endl;
      ++index;
    }
    os << "-----------\n";
    current = current->next;
  }
  return os;
}

template <typename T, int Size>
Lariat<T, Size>::Lariat()
: head_(nullptr), tail_(nullptr), size_(0), nodecount_(0), asize_(Size)
{
}

template <typename T, int Size>
Lariat<T, Size>::Lariat(Lariat const& rhs): head_(nullptr), tail_(nullptr)
{
}

template <typename T, int Size>
Lariat<T, Size>::~Lariat()
{
  auto* walker = head_;
  while (walker)
  {
    auto* tmp = walker->next;
    delete walker;
    walker = tmp;
  }
}

template <typename T, int Size>
// Insert an element into the data structure at the index, between the element
  // at [index - 1] and the element at [index]
void Lariat<T, Size>::insert(const int index, const T& value)
{
  // The first thing to this function is to check for an Out of Bounds error.
  if (index < 0 || index > size_)
  {
    LariatException(
      LariatException::LARIAT_EXCEPTION::E_BAD_INDEX,
      "Subscript is out of range"
    );
  }

  // Make sure to handle the "edge" cases, which allow for insertion at the end
    // of the deque as well as the beginning. I personally suggest calling
    // push_back and push_front in these cases, as it helps minimize the amount
    //  of code written and the algorithm is identical anyways.
  // end
  if (index == size_)
  {
    push_back(value);
  }

  // todo
  // beginning

  // todo
  // The next thing to do is to set up the actual insertion algorithm.
}

template <typename T, int Size>
void Lariat<T, Size>::push_back(const T& value)
{
  if (!head_ && !tail_)
  {
    head_ = allocate();
    tail_ = head_;
    add_value(head_, 0, value);
    return;
  }
  // If the tail node is full, split the node and update the tail_ pointer.
  // Set the last element in the tail's array to the value.
  // Increment the tail node's count.
  else if (tail_->count == asize_)
  {
    tail_ = split(tail_);
  }

  add_value(tail_, tail_->count, value);
}

template <typename T, int Size>
void Lariat<T, Size>::push_front(const T& value)
{
}

template <typename T, int Size>
void Lariat<T, Size>::erase(int index)
{
}

template <typename T, int Size>
void Lariat<T, Size>::pop_back()
{
}

template <typename T, int Size>
void Lariat<T, Size>::pop_front()
{
}

template <typename T, int Size>
T& Lariat<T, Size>::operator[](int index)
{
  return {};
}

template <typename T, int Size>
const T& Lariat<T, Size>::operator[](int index) const
{
  return {};
}

template <typename T, int Size>
T& Lariat<T, Size>::first()
{
  return {};
}

template <typename T, int Size>
T const& Lariat<T, Size>::first() const
{
  return {};
}

template <typename T, int Size>
T& Lariat<T, Size>::last()
{
  return {};
}

template <typename T, int Size>
T const& Lariat<T, Size>::last() const
{
  return {};
}

template <typename T, int Size>
unsigned Lariat<T, Size>::find(const T& value) const
{
  return 0;
}

template <typename T, int Size>
size_t Lariat<T, Size>::size() const
{
  return 0;
}

template <typename T, int Size>
void Lariat<T, Size>::clear()
{
}

template <typename T, int Size>
void Lariat<T, Size>::compact()
{
}

template <typename T, int Size>
typename Lariat<T, Size>::LNode* Lariat<T, Size>::allocate()
{
  ++nodecount_;
  return new LNode;
}

template <typename T, int Size>
void Lariat<T, Size>::add_value(LNode* node, size_t ind, T const& val)
{
    node->values[ind] = val;
    ++node->count;
    ++size_;
}

template <typename T, int Size>
void Lariat<T, Size>::remove_value(LNode* node/*, size_t ind, T const& val*/)
{
  //// todo
  //if (ind + 1 % asize_)
  //{
  //  //for
  //  //node->values[]
  //}

  --node->count;
  --size_;
}

template <typename T, int Size>
typename Lariat<T, Size>::LNode* Lariat<T, Size>::split(LNode* node)
{
  LNode* tmp = nullptr;
  if (node->next)
  {
    tmp = node->next;
  }
  auto* newNode = allocate();
  node->next = newNode;
  if (tmp)
  {
    newNode->next = tmp;
    tmp->prev = newNode;
  }

  for (auto i = 0; i < asize_ / 2; ++i)
  {
    if (asize_ % (asize_ / 2))
    {
      add_value(newNode, i, node->values[asize_ - (asize_ / 2) + i]);
    }
    else
    {
      add_value(newNode, i, node->values[asize_ - (asize_ / 2) + i - 1]);
    }

    remove_value(node);
  }

  return newNode;
}


#else // fancier 
#endif
