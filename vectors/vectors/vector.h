/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An iterator through Vector
 * Author
 *    Martin Nkala and Amber Robbins
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include <new>      // std::bad_alloc
#include <memory>   // for std::allocator
using namespace std;

class TestVector; // forward declaration for unit tests
class TestStack;
class TestPQueue;
class TestHash;

namespace custom
{

   /*****************************************
	* VECTOR
	* Just like the std :: vector <T> class
	****************************************/
   template <typename T, typename A = std::allocator<T>>
   class vector
   {
	  friend class ::TestVector; // give unit tests access to the privates
	  friend class ::TestStack;
	  friend class ::TestPQueue;
	  friend class ::TestHash;
   public:

	  //
	  // Construct
	  //
	  vector(const A& a = A());
	  vector(size_t numElements, const A& a = A());
	  vector(size_t numElements, const T& t, const A& a = A());
	  vector(const std::initializer_list<T>& l, const A& a = A());
	  vector(const vector& rhs);
	  vector(vector&& rhs);
	  ~vector();

	  //
	  // Assign
	  //
	  void swap(vector& rhs)
	  {
		 std::swap(data, rhs.data);
		 std::swap(numElements, rhs.numElements);
		 std::swap(numCapacity, rhs.numCapacity);
	  }
	  vector& operator = (const vector& rhs);
	  vector& operator = (vector&& rhs);

	  //
	  // Iterator
	  //
	  class iterator;
	  iterator begin() { return iterator(data); }
	  iterator end() { return iterator(data + numElements); }

	  //
	  // Access
	  //
	  T& operator [] (size_t index);
	  const T& operator [] (size_t index) const;
	  T& front();
	  const T& front() const;
	  T& back();
	  const T& back() const;

	  //
	  // Insert
	  //
	  void push_back(const T& t);
	  void push_back(T&& t);
	  void reserve(size_t newCapacity);
	  void resize(size_t newElements);
	  void resize(size_t newElements, const T& t);

	  //
	  // Remove
	  //
	  void clear()
	  {
		 for (int i = 0; i < size(); i++)
			alloc.destroy(data + i);
		 numElements = 0;
	  }
	  void pop_back()
	  {
		 if (numElements > 0)
		 {
			alloc.destroy(data + numElements - 1);
			numElements--;
		 }
	  }
	  void shrink_to_fit();

	  //
	  // Status
	  //
	  size_t  size()          const { return numElements; }
	  size_t  capacity()      const { return numCapacity; }
	  bool empty()            const { return numElements == 0; }

   private:
	  A    alloc;                // use allocator for memory allocation
	  T* data;                 // user data, a dynamically-allocated array
	  size_t  numCapacity;       // the capacity of the array
	  size_t  numElements;       // the number of items currently used
   };

   /**************************************************
	* VECTOR ITERATOR
	* An iterator through vector.  You only need to
	* support the following:
	*   1. Constructors (default and copy)
	*   2. Not equals operator
	*   3. Increment (prefix and postfix)
	*   4. Dereference
	* This particular iterator is a bi-directional meaning
	* that ++ and -- both work.  Not all iterators are that way.
	*************************************************/
   template <typename T, typename A>
   class vector <T, A> ::iterator
   {
	  friend class ::TestVector; // give unit tests access to the privates
	  friend class ::TestStack;
	  friend class ::TestPQueue;
	  friend class ::TestHash;
   public:
	  // constructors, destructors, and assignment operator
	  iterator() : p(nullptr) {  }
	  iterator(T* p) { this->p = p; }
	  iterator(const iterator& rhs) { *this = rhs; }
	  iterator(size_t index, vector<T>& v) { *this = v.data + index; }
	  iterator& operator = (const iterator& rhs)
	  {
		 this->p = rhs.p;
		 return *this;
	  }

	  // equals, not equals operator
	  bool operator != (const iterator& rhs) const { return this->p != rhs->p; }
	  bool operator == (const iterator& rhs) const { return this->p == rhs->p; }

	  // dereference operator
	  T& operator * () { return *p; }

	  // prefix increment
	  iterator& operator ++ ()
	  {
		 p++;
		 return *this;
	  }

	  // postfix increment
	  iterator operator ++ (int postfix)
	  {
		 iterator tmp(*this);
		 p += postfix;
		 return tmp;
	  }

	  // prefix decrement
	  iterator& operator -- ()
	  {
		 p--;
		 return *this;
	  }

	  // postfix decrement
	  iterator operator -- (int postfix)
	  {
		 iterator tmp(*this);
		 p -= postfix;
		 return tmp;
	  }

   private:
	  T* p;
   };


   /*****************************************
	* VECTOR :: NON-DEFAULT constructors
	* non-default constructor: set the number of elements,
	* construct each element, and copy the values over
	****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(const A& a)
   {
	  alloc = a;
	  data = nullptr;
	  numElements = 0;
	  numCapacity = 0;
   }


   /*****************************************
	* VECTOR :: NON-DEFAULT constructors
	* non-default constructor: set the number of elements,
	* construct each element, and copy the values over
	****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(size_t num, const T& t, const A& a)
   {
	  alloc = a;
	  if (num == 0)
	  {
		 data = nullptr;
		 numElements = 0;
		 numCapacity = 0;
	  }
	  else
	  {
		 data = alloc.allocate(num);
		 numElements = num;
		 numCapacity = num;
		 for (int i = 0; i < num; i++)
			alloc.construct(data + i, t);
	  }
   }

   /*****************************************
	* VECTOR :: INITIALIZATION LIST constructors
	* Create a vector with an initialization list.
	****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(const std::initializer_list<T>& l, const A& a)
   {
	  alloc = a;
	  if (l.size() == 0)
	  {
		 data = nullptr;
		 numElements = 0;
		 numCapacity = 0;
	  }
	  else
	  {
		 data = alloc.allocate(l.size());
		 numElements = l.size();
		 numCapacity = l.size();
		 for (int i = 0; i < l.size(); i++)
			alloc.construct(data + i, *(l.begin() + i));
	  }
   }

   /*****************************************
	* VECTOR :: NON-DEFAULT constructors
	* non-default constructor: set the number of elements,
	* construct each element, and copy the values over
	****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(size_t num, const A& a)
   {
	  alloc = a;
	  if (num == 0)
	  {
		 data = nullptr;
		 numElements = 0;
		 numCapacity = 0;
	  }
	  else
	  {
		 data = alloc.allocate(num);
		 numElements = num;
		 numCapacity = num;
		 for (int i = 0; i < num; i++)
			alloc.construct(data + i);
	  }
   }

   /*****************************************
	* VECTOR :: COPY CONSTRUCTOR
	* Allocate the space for numElements and
	* call the copy constructor on each element
	****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(const vector& rhs)
   {
	  if (!rhs.empty())
	  {
		 data = alloc.allocate(rhs.numElements);
		 numElements = rhs.numElements;
		 numCapacity = rhs.numElements;
		 for (int i = 0; i < rhs.numElements; i++)
			alloc.construct(data + i, rhs.data[i]);
	  }
	  else
	  {
		 data = nullptr;
		 numElements = 0;
		 numCapacity = 0;
	  }
   }

   /*****************************************
	* VECTOR :: MOVE CONSTRUCTOR
	* Steal the values from the RHS and set it to zero.
	****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(vector&& rhs)
   {
	  data = rhs.data;
	  numElements = rhs.numElements;
	  numCapacity = rhs.numCapacity;

	  rhs.data = nullptr;
	  rhs.numElements = 0;
	  rhs.numCapacity = 0;
   }

   /*****************************************
	* VECTOR :: DESTRUCTOR
	* Call the destructor for each element from 0..numElements
	* and then free the memory
	****************************************/
   template <typename T, typename A>
   vector <T, A> :: ~vector()
   {
	  if (!empty())
	  {
		 for (int i = 0; i < numElements; i++)
			alloc.destroy(data + i);
		 alloc.deallocate(data, numCapacity);
	  }
   }

   /***************************************
	* VECTOR :: RESIZE
	* This method will adjust the size to newElements.
	* This will either grow or shrink newElements.
	*     INPUT  : newCapacity the size of the new buffer
	*     OUTPUT :
	**************************************/
   template <typename T, typename A>
   void vector <T, A> ::resize(size_t newElements)
   {
	  // Resizing a vector to 0
	  if (newElements == 0)
	  {
		 clear();
		 return;
	  }

	  // Do nothing if the newElements are the same as the current
	  if (newElements == numElements)
		 return;

	  // Cut back if the newElements are smaller than the numElements
	  if (newElements < numElements)
	  {
		 for (size_t i = newElements; i < numElements; i++)
			alloc.destroy(data + i);
		 numElements = newElements;
	  }
	  // Increase capacity if the newElements are greater than the
	  // current capacity.
	  else
	  {
		 if (newElements > numCapacity)
			reserve(newElements);
		 for (size_t i = numElements; i < newElements; i++)
			alloc.construct(data + i);
		 numElements = newElements;
	  }
   }

   // Resize with two parameters
   template <typename T, typename A>
   void vector <T, A> ::resize(size_t newElements, const T& t)
   {
	  // Resizing a vector to 0
	  if (newElements == 0)
	  {
		 clear();
		 return;
	  }

	  // Do nothing if the newElements are the same as the current
	  if (newElements == numElements)
		 return;

	  // Cut back if the newElements are smaller than the numElements
	  if (newElements < numElements)
	  {
		 for (size_t i = newElements; i < numElements; i++)
			alloc.destroy(data + i);
		 numElements = newElements;
	  }

	  // Increase capacity if the newElements are greater than the
	  // current capacity.
	  else
	  {
		 if (newElements > numCapacity)
			reserve(newElements);
		 for (size_t i = numElements; i < newElements; i++)
			alloc.construct(data + i, t);
		 numElements = newElements;
	  }
   }

   /***************************************
	* VECTOR :: RESERVE
	* This method will grow the current buffer
	* to newCapacity.  It will also copy all
	* the data from the old buffer into the new
	*     INPUT  : newCapacity the size of the new buffer
	*     OUTPUT :
	**************************************/
   template <typename T, typename A>
   void vector <T, A> ::reserve(size_t newCapacity)
   {
	  // Do nothing if the newCapacity is 0
	  if (newCapacity == 0)
		 return;

	  // Copy all the data from the old buffer into the new
	  numCapacity = newCapacity;
	  size_t tmpNumElements = numElements;
	  T* tmp = alloc.allocate(numCapacity);
	  for (size_t i = 0; i < numElements; i++)
		 alloc.construct(tmp + i, std::move(data[i]));

	  // Cannot clear if data is null
	  if (data != nullptr)
	  {
		 clear();
		 alloc.deallocate(data, numCapacity);
	  }

	  // Update the data pointer and the number of elements
	  // to match the new capacity
	  data = tmp;
	  numElements = tmpNumElements;
   }

   /***************************************
	* VECTOR :: SHRINK TO FIT
	* Get rid of any extra capacity
	*     INPUT  :
	*     OUTPUT :
	**************************************/
   template <typename T, typename A>
   void vector <T, A> ::shrink_to_fit()
   {
	  // Reset all attributes if container is empty
	  if (empty())
	  {
		 data = nullptr;
		 numElements = 0;
		 numCapacity = 0;
		 return;
	  }

	  // Do nothing is the number of elements equals the capacity.
	  if (numElements == numCapacity)
		 return;

	  // Copy data to temporary buffer
	  size_t tmpNumElements = numElements;
	  T* tmp = alloc.allocate(numElements);
	  for (size_t i = 0; i < numElements; i++)
		 alloc.construct(tmp + i, data[i]);

	  clear();

	  // Deallocate the old buffer and assign the new buffer
	  alloc.deallocate(data, numCapacity);
	  data = tmp;
	  numCapacity = tmpNumElements;
	  numElements = tmpNumElements;
   }

   /*****************************************
	* VECTOR :: SUBSCRIPT
	* Read-Write access
	****************************************/
   template <typename T, typename A>
   T& vector <T, A> :: operator [] (size_t index)
   {
	  return data[index];
   }

   /******************************************
	* VECTOR :: SUBSCRIPT
	* Read-Write access
	*****************************************/
   template <typename T, typename A>
   const T& vector <T, A> :: operator [] (size_t index) const
   {
	  return data[index];
   }

   /*****************************************
	* VECTOR :: FRONT
	* Read-Write access
	****************************************/
   template <typename T, typename A>
   T& vector <T, A> ::front()
   {
	  return data[0];
   }

   /******************************************
	* VECTOR :: FRONT
	* Read-Write access
	*****************************************/
   template <typename T, typename A>
   const T& vector <T, A> ::front() const
   {
	  return data[0];
   }

   /*****************************************
	* VECTOR :: BACK
	* Read-Write access
	****************************************/
   template <typename T, typename A>
   T& vector <T, A> ::back()
   {
	  return data[numElements - 1];
   }

   /******************************************
	* VECTOR :: BACK
	* Read-Write access
	*****************************************/
   template <typename T, typename A>
   const T& vector <T, A> ::back() const
   {
	  return data[numElements - 1];
   }

   /***************************************
	* VECTOR :: PUSH BACK
	* This method will add the element 't' to the
	* end of the current buffer.  It will also grow
	* the buffer as needed to accomodate the new element
	*     INPUT  : 't' the new element to be added
	*     OUTPUT : *this
	**************************************/
   template <typename T, typename A>
   void vector <T, A> ::push_back(const T& t)
   {
	  // Allocate a space for the first element
	  if (numCapacity == 0)
	  {
		 data = alloc.allocate(1);
		 alloc.construct(data, t);
		 numElements = 1;
		 numCapacity = 1;
	  }

	  // Double capacity if the current capacity is full
	  else if (numElements == numCapacity)
	  {
		 reserve(numCapacity * 2);
		 alloc.construct(data + numElements++, t);
	  }

	  // If capacity not full, fill in the new element and
	  // increment the count
	  else
	  {
		 alloc.construct(data + numElements++, t);
	  }
   }

   template <typename T, typename A>
   void vector <T, A> ::push_back(T&& t)
   {
	  // Allocate a space for the first element
	  if (numCapacity == 0)
	  {
		 data = alloc.allocate(1);
		 alloc.construct(data, std::move(t));
		 numElements = 1;
		 numCapacity = 1;
	  }

	  // Double capacity if the current capacity is full
	  else if (numElements == numCapacity)
	  {
		 reserve(numCapacity * 2);
		 alloc.construct(data + numElements++, std::move(t));
	  }

	  // If capacity not full, fill in the new element and
	  // increment the count
	  else
	  {
		 alloc.construct(data + numElements++, std::move(t));
	  }
   }

   /***************************************
	* VECTOR :: ASSIGNMENT
	* This operator will copy the contents of the
	* rhs onto *this, growing the buffer as needed
	*     INPUT  : rhs the vector to copy from
	*     OUTPUT : *this
	**************************************/
   template <typename T, typename A>
   vector <T, A>& vector <T, A> :: operator = (const vector& rhs)
   {
	  // If rhs is empty, do nothing
	  if (rhs.empty())
		 return *this;

	  // If size is equal copy eleements from rhs
	  if (numElements == rhs.size())
	  {
		 for (int i = 0; i < size(); i++)
			data[i] = rhs.data[i];
	  }

	  // If suffiecient space available copy elements from rhs
	  // and fill in the remaining space if needed
	  else if (numElements < rhs.size())
	  {
		 if (numCapacity >= rhs.size())
		 {
			for (int i = 0; i < numElements; i++)
			   data[i] = rhs.data[i];
			for (size_t i = numElements; i < rhs.size(); i++)
			   alloc.construct(data + i, rhs.data[i]);
		 }

		 // If insufficient space, reallocate and copy elements from rhs
		 else
		 {
			T* tmp = alloc.allocate(rhs.size());
			for (size_t i = 0; i < rhs.size(); i++)
			   alloc.construct(tmp + i, rhs.data[i]);

			clear();
			alloc.deallocate(data, numCapacity);
			data = tmp;
			numCapacity = rhs.size();
		 }
	  }

	  // If rhs less than current vector, remove excess elements
	  else
	  {
		 for (int i = 0; i < rhs.size(); i++)
			data[i] = rhs.data[i];

		 for (size_t i = rhs.size(); i < size(); i++)
			alloc.destroy(data + i);
	  }
	  numElements = rhs.size();
	  return *this;
   }

   template <typename T, typename A>
   vector <T, A>& vector <T, A> :: operator = (vector&& rhs)
   {
	  // If rhs empty, reset attributes
	  if (rhs.empty())
	  {
		 data = nullptr;
		 numElements = 0;
		 numCapacity = 0;
	  }

	  // If rhs not empty, swap, then deallocate elememts from rhs
	  else
	  {
		 swap(rhs);
		 rhs.clear();
		 rhs.alloc.deallocate(rhs.data, rhs.capacity());

		 // Reset attributes
		 rhs.data = nullptr;
		 rhs.numElements = 0;
		 rhs.numCapacity = 0;
	  }
	  return *this;
   }

} // namespace custom

