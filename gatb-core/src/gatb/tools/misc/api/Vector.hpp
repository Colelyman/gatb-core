/*****************************************************************************
 *   GATB : Genome Assembly Tool Box                                         *
 *   Authors: [R.Chikhi, G.Rizk, E.Drezen]                                   *
 *   Based on Minia, Authors: [R.Chikhi, G.Rizk], CeCILL license             *
 *   Copyright (c) INRIA, CeCILL license, 2013                               *
 *****************************************************************************/

/** \file Vector.hpp
 *  \date 01/03/2013
 *  \author edrezen
 *  \brief Vector implementation
 */

#ifndef _GATB_CORE_TOOLS_MISC_VECTOR_HPP_
#define _GATB_CORE_TOOLS_MISC_VECTOR_HPP_

/********************************************************************************/

#include <gatb/tools/designpattern/api/SmartPointer.hpp>

/********************************************************************************/
namespace gatb      {
namespace core      {
namespace tools     {
namespace misc      {
/********************************************************************************/

/** \brief Vector class allowing to define vectors as sub parts of referred vectors.
 *
 * This vector implementation supports two kind of memory management:
 *      - dynamic allocation: the data buffer is allocated
 *      - reference : the data buffer is a reference to some existing buffer in memory
 */
template<typename T> class Vector : public dp::SmartPointer
{
public:

    /** Default constructor. */
    Vector () : _buffer(0), _size(0), _isAllocated(false), _ref(0)  {}

    /** Constructor with initial size.
     * \param aSize : initial size of the vector. */
    Vector (size_t aSize) :  _buffer(0), _size(0), _isAllocated(false), _ref(0)   {  resize (aSize);  }

    /** Destructor. */
    ~Vector ()
    {
        if (_isAllocated && _buffer) {  free (_buffer); }

        /** We get rid of the referred data if any. */
        setRef (0);
    }

    /** \return buffer holding the actual data. */
    char* getBuffer () const  { return _buffer; }

    /** \return buffer size (in bytes). */
    size_t size ()  const  { return _size; }

    /** Data access
     * \param[in] idx : index of the character to be retrieved
     * \return the retrieved character. */
    T& operator[]  (size_t idx)  { return _buffer[idx]; }

    /** Resize the current vector.
     * \param[in] aSize : new size of the vector. */
    void resize (size_t aSize)
    {
        _size        = aSize;
        _buffer      = (char*) realloc (_buffer, _size*sizeof(char));
        _isAllocated = true;
    }

    /** Set the size of the vector.
     * \param[in] size : new size of the vector. */
    void setSize (size_t size)  {  _size = size;  }

    /** Set the current data as a part of a referenced another data.
     * \param[in] ref : data referred by the current instance.
     * \param[in] offset : the current data will begin 'offset' bytes from the beginning of the referred data.
     * \param[in] length : size of the data */
    void setRef (Vector* ref, size_t offset, size_t length)
    {
        setRef (ref);
        _buffer      = _ref->_buffer + offset;
        _size        = length;
        _isAllocated = false;
    }

    /** Set the current data as a part of a referenced another data.
     * \param[in] offset : the current data will begin 'offset' bytes from the beginning of the referred data.
     * \param[in] length : size of the data */
    void setRef (T* buffer, size_t length)
    {
        _buffer      = buffer;
        _size        = length;
        _isAllocated = false;
    }

    /** Set the current data. Copy is done.
     * \param[in] buffer : data referred by the current instance.
     * \param[in] length : size of the data */
    void set (T* buffer, size_t length)
    {
        _size        = length;
        _buffer      = (char*) realloc (_buffer, _size*sizeof(char));
        _isAllocated = true;
        memcpy (_buffer, buffer, _size*sizeof(char));
    }

private:

    char*  _buffer;
    int    _size;
    bool   _isAllocated;

    Vector* _ref;
    void setRef (Vector* ref)  {  SP_SETATTR(ref);  }
};

/********************************************************************************/
} } } } /* end of namespaces. */
/********************************************************************************/

#endif /* _GATB_CORE_TOOLS_MISC_VECTOR_HPP_ */
