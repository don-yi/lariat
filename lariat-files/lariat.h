///////////////////////////////////////////////////////////////////////////////
#ifndef LARIAT_H
#define LARIAT_H
///////////////////////////////////////////////////////////////////////////////

#include <string>     // error strings
#include <utility>    // error strings
//#include <cstring>     // memcpy

class LariatException : public std::exception {  // NOLINT(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
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

  // ReSharper disable once CppOverridingFunctionWithoutOverrideSpecifier
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
class Lariat  // NOLINT(hicpp-special-member-functions)
{
public:
  Lariat();                  // default constructor                        
  Lariat(Lariat const& rhs); // copy constructor
  ~Lariat(); // destructor

  template<typename U, int Size2>
  explicit Lariat(Lariat<U, Size2> const& rhs); // copy constructor

  Lariat& operator=(Lariat const& rhs);

  template<typename U, int Size2>
  Lariat& operator=(Lariat<U, Size2> const& rhs);

  friend class Lariat;

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
  struct LNode  // NOLINT
  {
    LNode* next = nullptr;
    LNode* prev = nullptr;
    int    count = 0;         // number of items currently in the node
    T values[Size];
  };
  LNode* head_;           // points to the first node
  LNode* tail_;           // points to the last node
  int size_{};              // the number of items (not nodes) in the list
  mutable int nodecount_{}; // the number of nodes in the list
  int asize_;             // the size of the array within the nodes

  LNode* allocate();
  void deallocate(LNode* deleteFrom);
  void add_value(LNode* node, int ind, T const& val);
  void remove_value(LNode* node);
  void kick_start();
  LNode* split(LNode* node, bool isPushBack = false);
  LNode* split(LNode* node, int ind, T const& val);
  void shift_up(LNode* node, int ind);
  void shift_down(LNode* node, int ind);
  static int find_element(int ind, LNode** localNode);
  void link(LNode* prev, LNode* next);
  void copy_values(LNode* original, LNode* copy);
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
: head_(nullptr), tail_(nullptr), asize_(Size)
{
}

// This is the standard copy constructor. The function should loop through the
  // instance passed in, pushing each element of the other onto the back of
  // the one being constructed.
// It should be done using the algorithm for (or directly calling) push_back
  // so that all of the nodes are split correctly.
template <typename T, int Size>
Lariat<T, Size>::Lariat(Lariat const& rhs): head_(nullptr), tail_(nullptr)
{
  asize_ = rhs.asize_;

  auto* walker = rhs.head_;
  while (walker)
  {
    for (auto i = 0; i < walker->count; i++)
    {
      push_back(walker->values[i]);
    }
    walker = walker->next;
  }
}

template <typename T, int Size>
template <typename U, int Size2>
Lariat<T, Size>::Lariat(Lariat<U, Size2> const& rhs)
: head_(nullptr), tail_(nullptr)
{
  asize_ = Size;

  auto* walker = rhs.head_;
  while (walker)
  {
    for (auto i = 0; i < walker->count; i++)
    {
      push_back(static_cast<U>(walker->values[i]));
    }
    walker = walker->next;
  }
}

template <typename T, int Size>
Lariat<T, Size>::~Lariat()
{
  deallocate(head_);
}

template <typename T, int Size>
Lariat<T, Size>& Lariat<T, Size>::operator=(Lariat const& rhs)
{
  // Set the non-pointer members as necessary, clear this instance's
    // data, then walk through the right-hand argument's list adding each
    // element to this instance
  asize_ = rhs.asize_;
  clear();

  auto* walker = rhs.head_;
  while (walker)
  {
    for (auto i = 0; i < walker->count; i++)
    {
      push_back(walker->values[i]);
    }
    walker = walker->next;
  }

  return *this;
}

template <typename T, int Size>
template <typename U, int Size2>
Lariat<T, Size>& Lariat<T, Size>::operator=(Lariat<U, Size2> const& rhs)
{
  asize_ = Size;
  clear();

  auto* walker = rhs.head_;
  while (walker)
  {
    for (auto i = 0; i < walker->count; i++)
    {
      push_back(walker->values[i]);
    }
    walker = walker->next;
  }

  return *this;
}

template <typename T, int Size>
// Insert an element into the data structure at the index, between the element
// at [index - 1] and the element at [index]
void Lariat<T, Size>::insert(const int index, const T& value)
{
  // The first thing to this function is to check for an Out of Bounds error.
  if (index < 0 || index > size_)
  {
    throw LariatException(
      LariatException::LARIAT_EXCEPTION::E_BAD_INDEX,
      "Subscript is out of range"
    );
  }
  // Make sure to handle the "edge" cases, which allow for insertion at the end
  // of the deque as well as the beginning.
  else if (index == size_)
  {
    push_back(value);
  }
  else if (index == 0)
  {
    push_front(value);
  }
  // The next thing to do is to set up the actual insertion algorithm.
  else
  {
    // First, find the node and local index of the element being inserted.
    LNode* localNode = head_;
    const auto localInd = find_element(index, &localNode);

    // If the node is full, you will need to keep track of the last element in
    // the node before you shift all the elements to the right.
    if (localNode->count == asize_)
    {
      // Next, you will need to split the node.
      // You will need to transfer the overflow to the last element of the new
      // node created by the split.
      split(localNode, localInd, value);
    }
    else
    {
      // Next, shift all elements past that local index one element to the right.
      shift_up(localNode, localInd);
      add_value(localNode, localInd, value);
    }
  }
}

template <typename T, int Size>
void Lariat<T, Size>::push_back(const T& value)
{
  if (!head_)
  {
    kick_start();
  }
  // If the tail node is full, split the node and update the tail_ pointer.
  // Set the last element in the tail's array to the value.
  // Increment the tail node's count.
  else if (tail_->count == asize_)
  {
    link(tail_, split(tail_, true));
  }

  add_value(tail_, tail_->count, value);
}

template <typename T, int Size>
void Lariat<T, Size>::push_front(const T& value)
{
  // If the head node is empty, increment the node's count.
  if (!head_)
  {
    kick_start();
  }
  // If the head node is full, you will need to shift the elements up, in the
  // same way they were shifted in the insert function, making sure to track
  // the overflow.
  // Next you will need to split the node.
  else if (head_->count == asize_)
  {
    // In order to account for splitting the only node in the linked list, you
    // will have to update the tail_ pointer as necessary.
    auto* oldHead = head_;
    link(head_, split(head_));

    //// Move the half of the values.
    //auto numMv = asize_ / 2;
    //// Handle even array size.
    //if (!(asize_ % 2))
    //{
    //  --numMv;
    //}
    //for (auto i = 0; i < numMv; ++i)
    //{
      shift_up(oldHead, 0);
    //}
  }
  // If the head node isn't full yet, just shift the head node up an element
  // from element 0 and increase the count.
  else if (head_->count < asize_)
  {
    shift_up(head_, 0);
  }

  // Set the 0'th element of the head to the value.
  add_value(head_, 0, value);
}

template <typename T, int Size>
void Lariat<T, Size>::erase(const int index)
{
  // You can use the pop_back and pop_front functions if the index requested is
    // the first or last element.
  if (index == size_)
  {
    pop_back();
    return;
  }
  if (index == 0)
  {
    pop_front();
    return;
  }

  // First, find the containing node and local index of the requested global
    // index.
  LNode* localNode = head_;
  const auto localInd = find_element(index, &localNode);

  // Shift all the elements in the node beyond the local index left one element,
    // covering the element being erased.
  shift_down(localNode,localInd);
}

template <typename T, int Size>
void Lariat<T, Size>::pop_back()
{
  // Decrement the count of the tail node.
  --tail_->count;
  --size_;
}

template <typename T, int Size>
void Lariat<T, Size>::pop_front()
{
  // Shift all elements in the head node down one element.
  // Decrement the head's count.
  shift_down(head_, 0);

  // If the head node is now empty, free the associated memory.
  if (head_->count <= 0)
  {
    auto* emptyHead = head_;
    head_ = head_->next;
    delete emptyHead;
  }
}

template <typename T, int Size>
T& Lariat<T, Size>::operator[](const int index)
{
  auto* localNode = head_;
  const auto localInd = find_element(index, &localNode);
  return localNode->values[localInd];
}

template <typename T, int Size>
const T& Lariat<T, Size>::operator[](const int index) const
{
  auto* localNode = head_;
  const auto localInd = find_element(index, &localNode);
  return localNode->values[localInd];
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
  // Walk the list in a similar fashion to that detailed in the findElement
    // helper function, but check equivalence for each element in each node,
    // returning the index when the desired element is found.
  auto* walker = head_;
  auto ind = -1;
  while (walker)
  {
    for (auto i = 0; i < walker->count; ++i)
    {
      ++ind;
      if (walker->values[i] == value)
      {
        return static_cast<unsigned>(ind);
      }
    }
    walker = walker->next;
  }

  // If the desired element is not found, return the total number of elements
    // contained in the data structure.
  return size_;
}

template <typename T, int Size>
size_t Lariat<T, Size>::size() const
{
  return size_;
}

template <typename T, int Size>
void Lariat<T, Size>::clear()
{
  size_ = 0;

  auto* walker = head_;
  while (walker)
  {
    walker->count = 0;
    walker = walker->next;
  }
}

// Compact takes all the data stored in the linked list and moves it into the
  // smallest number of nodes possible. Then it frees all empty nodes at the
  // end of the list.
template <typename T, int Size>
void Lariat<T, Size>::compact()
{
  // Handle empty lariat.
  if (!head_)
  {
    return;
  }

  // First, loop through the list with both feet until the left node is not
    // already full and the right foot hasn't walked off the list.
  auto* lft = head_;
  auto* rft = head_->next;
  while (lft->count == asize_ && rft)
  {
    rft = rft->next;
    lft = lft->next;
  }

  // Next, walk through the list while the right foot hasn't lost the list.
  while (rft)
  {
    // This loop should have a nested loop that stores each value from the right
      // foot in the left foot's node, and steps the left foot to the next node
      // when it is filled.
    const auto rtot = rft->count;
    size_ -= rft->count;
    rft->count = 0;
    for (auto i = 0; i < rtot; ++i)
    {
      add_value(lft, lft->count, rft->values[i]);
      
      if (lft->count == asize_)
      {
        lft = lft->next;
      }
    }
    rft = rft->next;
  }

  // Delete empty nodes.
  if (lft->count == 0)
  {
    deallocate(lft);
  }
  else if (lft->next)
  {
    deallocate(lft->next);
  }
}

template <typename T, int Size>
typename Lariat<T, Size>::LNode* Lariat<T, Size>::allocate()
{
  ++nodecount_;
  return new LNode;
}

template <typename T, int Size>
void Lariat<T, Size>::deallocate(LNode* deleteFrom)
{
  // Handle the tail pointer.
  if (deleteFrom->prev)
  {
    tail_ = deleteFrom->prev;
    deleteFrom->prev->next = nullptr;
  }
  // Handle the head pointer.
  else if (deleteFrom == head_)
  {
    head_ = nullptr;
    tail_ = nullptr;
  }

  // Walk and delete.
  auto* walker = deleteFrom;
  while (walker)
  {
    auto* tmp = walker->next;
    delete walker;
    walker = tmp;
  }
}

template <typename T, int Size>
void Lariat<T, Size>::add_value(LNode* node, int ind, T const& val)
{
    node->values[ind] = val;
    ++node->count;
    ++size_;
}

template <typename T, int Size>
void Lariat<T, Size>::remove_value(LNode* node)
{
  --node->count;
  --size_;
}

template <typename T, int Size>
void Lariat<T, Size>::kick_start()
{
  head_ = allocate();
  tail_ = head_;
}

template <typename T, int Size>
typename Lariat<T, Size>::LNode* Lariat<T, Size>::split(
  LNode* node, const bool isPushBack
)
{
  auto* newNode = allocate();

  // Move the half of the values.
  auto numMv = asize_ / 2;
  if (isPushBack && !(asize_ % 2))
  {
    --numMv;
  }
  for (auto i = 0; i < numMv; ++i)
  {
    add_value(newNode, i, node->values[asize_ - numMv + i]);
    remove_value(node);
  }

  return newNode;
}

template <typename T, int Size>
typename Lariat<T, Size>::LNode* Lariat<T, Size>::split(
  LNode* node, const int ind, T const & val
)
{
  // Allocate and link the new node.
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
  else
  {
    tail_ = newNode;
  }

  // Move the values according to the inserting index.
  if (ind <= asize_ / 2)
  {
    // Handle number of moves for even and odd array size.
    auto numMv = asize_ / 2;
    if (asize_ % 2)
    {
      ++numMv;
    }

    for (auto i = 0; i < numMv; ++i)
    {
      add_value(newNode, i, node->values[asize_ - numMv + i]);
      remove_value(node);
    }
    //add_value(newNode, i, poppedElem);
    shift_up(node, ind);
    add_value(node, ind, val);
  }
  else
  {
    for (auto i = 0; i < asize_ / 2 - 1; ++i)
    {
      add_value(newNode, i, node->values[asize_ - asize_ / 2 + i + 1]);
      remove_value(node);
    }
    //add_value(newNode, i, poppedElem);
    auto const localInd = ind - asize_ / 2 - 1;
    shift_up(newNode, localInd);
    add_value(newNode, localInd, val);
  }

  return newNode;
}

template <typename T, int Size>
void Lariat<T, Size>::shift_up(LNode* node, int ind)
{
  for (auto i = node->count - 1; i >= ind; --i)
  {
    node->values[i + 1] = node->values[i];
  }
}

template <typename T, int Size>
void Lariat<T, Size>::shift_down(LNode* node, const int ind)
{
  for (auto i = ind; i < node->count - 1; ++i)
  {
    node->values[i] = node->values[i + 1];
  }
  --node->count;
  --size_;
}

template <typename T, int Size>
int Lariat<T, Size>::find_element(const int ind, LNode** localNode)
{
  auto localInd = ind;
  while ((*localNode)->next && localInd >= 0 && localInd >= (*localNode)->count)
  {
    localInd -= (*localNode)->count;
    *localNode = (*localNode)->next;
  }

  return localInd;
}

template <typename T, int Size>
void Lariat<T, Size>::link(LNode* prev, LNode* next)
{
  next->next = prev->next;
  if (next->next)
  {
    next->next->prev = next;
  }
  prev->next = next;
  next->prev = prev;

  if (next == head_)
  {
    head_ = prev;
  }
  if (prev == tail_)
  {
    tail_ = next;
  }
}

template <typename T, int Size>
void Lariat<T, Size>::copy_values(LNode* original, LNode* copy)
{
  for (auto i = 0; i < original->count; ++i)
  {
    add_value(copy, i, original->values[i]);
  }
}

#else // fancier 
#endif
