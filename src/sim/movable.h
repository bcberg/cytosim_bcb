// Cytosim was created by Francois Nedelec. Copyright 2007-2017 EMBL.

#ifndef MOVABLE_H
#define MOVABLE_H

#include "vector.h"
#include "isometry.h"

class Space;
class Modulo;


/// Can be moved and rotated in space
/**
This provides a common interface, to move and rotation object in space.
The actual operations need to be implemented by redefining the virtual functions:
 
 if  mobile() == 0:
    the object has no position defined
 
 if  mobile() == 1:
    position() is implemented
    translate() is implemented
 
 if  mobile() == 2:
    rotate() is implemented
 
 if  mobile() == 3:
    position() is implemented
    translate() and rotate() are implemented
 .
 To support periodic boundary conditions, foldPosition() should be defined.
 */
class Movable
{
    
public:
    
    /// read a position specified with primitives, such as 'circle 5', etc.
    static Vector readPrimitive(std::istream&, Space const*);
    
    /// read a position in space
    static Vector readPosition(std::istream&, Space const*);

    /// read an orientation, and return a normalized vector
    static Vector readDirection(std::istream&, Vector const&, Space const*);

    /// read a rotation specified in stream, at position `pos`
    static Rotation readRotation(std::istream&, Vector const&, Space const*);
    
public:
    
    /// constructor
    Movable() {}
    
    
    /// true if object can be translated (default=false)
    /**
     if  mobile() == 0:
         the object has no position defined
     
     if  mobile() == 1:
         position() is implemented
         translate() is implemented
     
     if  mobile() == 2:
         rotate() is implemented
     
     if  mobile() == 3:
         position() is implemented
         translate() and rotate() are implemented
     */
    virtual int       mobile()  const { return 0; }
    
    /// return the spatial position of the Object
    virtual Vector    position()  const { return Vector(0.0,0.0,0.0); }
    
    /// move Object ( position += given vector )
    virtual void      translate(Vector const&);
    
    /// move Object to specified position
    virtual void      setPosition(Vector const& x) { translate( x - position() ); }

    /// translate Object by a rotation around the Origin
    void              rotateT(Rotation const&);
    
    /// rotate Object around the Origin
    virtual void      rotate(Rotation const&);
    
    /// rotate Object around its current position, using translate() and rotate()
    void              revolve(Rotation const&);
    
    
    /// perform modulo for periodic boundary conditions
    /** This brings the object to the centered mirror image defined by Modulo*/
    virtual void      foldPosition(Modulo const*) {}
    
};


#endif

