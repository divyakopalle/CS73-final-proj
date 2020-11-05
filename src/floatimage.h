/*!
    \file floatimage.h
    \brief Contains the definition of a floating-point image class with an arbitrary number of channels
    \author Wojciech Jarosz
    
    CS 73/273 Computational Aspects of Digital Photography C++ basecode.
*/
#pragma once

#include "array3D.h"
#include <string>


//! Floating point image
class FloatImage : public Array3D<float>
{
public:
    //-----------------------------------------------------------------------
    //@{ \name Constructors, destructors, etc.
    //-----------------------------------------------------------------------
    FloatImage();
    FloatImage(int width, int height, int channels);
    FloatImage(const std::string & filename);
    FloatImage(const FloatImage &other);
    FloatImage & operator=(const FloatImage &other);
    void clear(float v = 0.0f) {reset(v);}
    void clear(const std::vector<float> & channelValues);
    //@}

    int channels() const {return m_sizeZ;}



    //-----------------------------------------------------------------------
    //@{ \name Image I/O.
    //-----------------------------------------------------------------------
    bool read(const std::string &name);
    bool write(const std::string &name) const;
    bool debugWrite() const; // Writes image to Output directory with automatically chosen name
    //@}

private:
    static int s_debugWriteNumber; // Image number for debug write
};
