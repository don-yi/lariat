//#include <iostream>
////#include <iomanip>
//
//#if 1
//template <typename T, int Size>
//std::ostream& operator<<(std::ostream &os, Lariat<T, Size> const & rhs)
//{
//  typename Lariat<T, Size>::LNode * current = rhs.head_;
//  int index = 0;
//  while (current) {
//    os << "Node starting (count " << current->count << ")\n";
//    for (int local_index = 0; local_index < current->count; ++local_index) {
//      os << index << " -> " << current->values[local_index] << std::endl;
//      ++index;
//    }
//    os << "-----------\n";
//    current = current->next;
//  }
//  return os;
//}
//
//template <typename T, int Size>
//Lariat<T, Size>::Lariat(): head_(nullptr), tail_(nullptr)
//{
//}
//
//template <typename T, int Size>
//Lariat<T, Size>::Lariat(Lariat const& rhs): head_(nullptr), tail_(nullptr)
//{
//}
//
//template <typename T, int Size>
//Lariat<T, Size>::~Lariat()
//= default;
//
//template <typename T, int Size>
//void Lariat<T, Size>::insert(int index, const T& value)
//{
//}
//
//template <typename T, int Size>
//void Lariat<T, Size>::push_back(const T& value)
//{
//}
//
//template <typename T, int Size>
//void Lariat<T, Size>::push_front(const T& value)
//{
//}
//
//template <typename T, int Size>
//void Lariat<T, Size>::erase(int index)
//{
//}
//
//template <typename T, int Size>
//void Lariat<T, Size>::pop_back()
//{
//}
//
//template <typename T, int Size>
//void Lariat<T, Size>::pop_front()
//{
//}
//
//template <typename T, int Size>
//T& Lariat<T, Size>::operator[](int index)
//{
//  return {};
//}
//
//template <typename T, int Size>
//const T& Lariat<T, Size>::operator[](int index) const
//{
//  return {};
//}
//
//template <typename T, int Size>
//T& Lariat<T, Size>::first()
//{
//  return {};
//}
//
//template <typename T, int Size>
//T const& Lariat<T, Size>::first() const
//{
//  return {};
//}
//
//template <typename T, int Size>
//T& Lariat<T, Size>::last()
//{
//  return {};
//}
//
//template <typename T, int Size>
//T const& Lariat<T, Size>::last() const
//{
//  return {};
//}
//
//template <typename T, int Size>
//unsigned Lariat<T, Size>::find(const T& value) const
//{
//  return 0;
//}
//
//template <typename T, int Size>
//size_t Lariat<T, Size>::size() const
//{
//  return 0;
//}
//
//template <typename T, int Size>
//void Lariat<T, Size>::clear()
//{
//}
//
//template <typename T, int Size>
//void Lariat<T, Size>::compact()
//{
//}
//
//
//#else // fancier 
//#endif
