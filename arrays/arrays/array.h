/***********************************************************************
 * Header:
 *    Array
 * Summary:
 *    Our custom implementation of std::array
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *       array             : similar to std::array
 *       array :: iterator : an iterator through the array
 * Author
 *    Amber Robbins
 * ************************************************************************/

#pragma once

#include <stdexcept>

class TestArray;

namespace custom
{

/************************************************
 * ARRAY
 * A class that holds stuff
 ***********************************************/
template <int N>
struct array
{
public:
   //
   // CONSTRUCT
   //

   // No constructors or destructors needed here

   //
   // ASSIGN
   //

   // No assignment operators needed here
	
   //
   // MEMBER VARIABLE
   //

   // statically allocated array of T
   int __elems_[N >= 0 ? N : 999];

   //
   // ITERATORS
   //

   // Forward declaration for the iterator class
   class iterator;

   // returns an iterator referring to the first element in the array
   iterator begin()
   {
	  return iterator(__elems_);
   }
	
   // returns an iterator referring to the first element off the end of the array
   iterator end()
   {
	  return iterator(__elems_ + size());
   }
	
	//
	// STATUS
	//

	// number of elements in the array
	size_t size()  const
	{
	   if (N >= 0)
		   return N;
		return 999;
	}
	 
	// returns true if no elements in array
	bool empty() const
	{
		if (size() < 1)
			return true;
		return false;
	}

   //
   // ACCESS METHODS
   //
	
	// return the first element in the array
	int& front()
	{
		return __elems_[0];
	}
	
	// return the last element in the array
	int& back()
	{
		return __elems_[size() - 1];
	}
	
	
	// NOTE: Try to ask teacher why implementation is the
	// same as the two non-const methods and iterators above
	const int& front() const
	{
		return __elems_[0];
	}
	
	const int& back()  const
	{
		return __elems_[size() - 1];
	}

   // Subscript - returns the i-th element in the array w/o bounds checking
   int& operator [] (size_t index)
   {
	   return __elems_[index];
   }
   const int& operator [] (size_t index) const
   {
	   return __elems_[index];
   }

   // At - returns the i-th element in the array with bounds checking
   int& at(size_t index)
   {
	   if (index >= 0 && index < size())
		   return __elems_[index];
	   else
		   throw std::out_of_range("ERROR: Index out of range");
   }
	
   const int& at(size_t index) const
   {
	   if (index >= 0 && index < size())
		   return __elems_[index];
	   else
		   throw std::out_of_range("ERROR: Index out of range");
   }

   //
   // Remove

   // No clear or erase here

};


/**************************************************
 * ARRAY ITERATOR
 * An iterator through array
 *************************************************/
template <int N>
class array <N> :: iterator
{
   friend class ::TestArray;
public:

   // default constructor
   iterator() : p(nullptr) {}
	
   // constructor
   iterator(int * p) { this->p = p; }
	
   // copy constructor
	iterator(const iterator & rhs) { this->p = rhs.p; }
	
   // assignment operator
   iterator & operator = (const iterator & rhs)
   {
	  this->p = rhs.p;
	  return *this;
   }
   
   // not equals operator
   bool operator != (const iterator & rhs) const
   {
	   if (this->p != rhs.p)
		   return true;
	   return false;
   }
	
   // equals operator
   bool operator == (const iterator & rhs) const
   {
	   if (this->p == rhs.p)
		   return true;
	   return false;
   }
   
   // dereference operator - fetches the element referred to by p
   int & operator * ()
   {
	  return *p;
   }
	
   // const dereference operator
   const int & operator * () const
   {
	   return *p;
   }
   
   // prefix increment
   iterator & operator ++ ()
   {
	   p += 1;
	   return *this;
   }
   
   // postfix increment
   iterator operator ++ (int postfix)
   {
	   iterator pTemp = *this;
	   p += 1;
	  return pTemp;
   }
   
private:
   int * p;
};



}; // namespace custom

