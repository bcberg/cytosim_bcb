// Cytosim was created by Francois Nedelec. Copyright 2007-2017 EMBL.

#ifndef FIBER_SITE_H
#define FIBER_SITE_H

#include "assert_macro.h"
#include "interpolation.h"
#include "fiber.h"
#include "sim.h"


/// FiberSite indicates a location on a Fiber by its abscissa from the Fiber's origin
/**
 The key variable is a pointer to a Fiber, `fbFiber`, which can be NULL for instance
 if the state is `unattached`.
 
 In the `attached` state, the precise location on the Fiber is recorded using the
 curvilinear abscissa `fbAbs`, measured along the fiber, from a reference
 that is fixed on the Fiber, called the Fiber's origin. This origin is virtual
 and may reside outside the Fiber ends.
 
 Importantly, the value of abscissa is independent from the vertices used to
 represent the Fiber's position, and also unaffected by assembly/disassembly
 at the tips of the Fiber.
 
 The `FiberSite` also support discrete binding, if the Fiber have a Lattice,
 and in this case uses an integer `fbSite` to keep track of the position.
 
 A `FiberSite` uses Interpolation to calculate its position in space.
*/
class FiberSite
{
    //friend class Hand;

private:
    
    /// the interpolation on the Fiber's vertices
    /**
     If all is well, ( inter.mecable() == fbFiber ) and ( abscissaInter() == fbAbs )
     */
    Interpolation inter;
    
    /// the abscissa of the interpolated point, which should be equal to `abscissa()`
    real abscissaInter() const { return fbFiber->abscissaPoint(inter.point1()+inter.coef1()); }

protected:
    
    /// the Fiber of interest, or NULL
    Fiber *       fbFiber;
    
    /// the abscissa from the origin of the Fiber
    real          fbAbs;
    
    /// propagate Lattice cell index type
    typedef FiberLattice::site_t site_t;
    
#if FIBER_HAS_LATTICE
    /// pointer to the Lattice of the Fiber, or NULL if not in use
    FiberLattice* fbLattice;
    
    /// index in the Fiber's Lattice
    site_t        fbSite;
#endif

public:

#if FIBER_HAS_LATTICE
    /// default constructor
    FiberSite() : fbFiber(nullptr), fbAbs(0), fbLattice(nullptr), fbSite(0) {}
#else
    FiberSite() : fbFiber(nullptr), fbAbs(0) {}
#endif

    /// construct at the given distance from the origin
    FiberSite(Fiber*, real a);

    /// make destructor non-virtual
    ~FiberSite() {}
    
#if FIBER_HAS_LATTICE
    
    /// return Lattice if engaged
    FiberLattice* lattice() const { return fbLattice; }
    
    /// index of Lattice's site
    site_t        site()    const { return fbSite; }
    
    /// set FiberLattice pointer at site `s` and abscissa `a`
    void engageLattice(FiberLattice* l, site_t s, real a)
    {
        fbLattice = l;
        fbSite    = s;
        fbAbs     = a;
        //assert_true(fbFiber->abscissaM() < a + REAL_EPSILON);
        //assert_true(a < fbFiber->abscissaP() + REAL_EPSILON);
    }

#else
    
    FiberLattice* lattice() const { return nullptr; }

#endif
    //--------------------------------------------------------------------------

    /// return the interpolation
    const Interpolation& interpolation() const { assert_false(bad()); return inter; }
    
    /// recalculate the Interpolation
    void         update()       { inter = fbFiber->interpolate(fbAbs); }
    
    /// move to a different abscissa on the current fiber
    void         moveTo(real a) { fbAbs = a; update(); }

    /// relocate to MINUS_END of current fiber
    void         relocateM();
    
    /// relocate to PLUS_END of current fiber
    void         relocateP();

    //--------------------------------------------------------------------------
    
    /// true if not attached
    bool         unattached()    const { return !fbFiber; }

    /// true if attached
    bool         attached()      const { return fbFiber; }
    
    /// Fiber to which this is attached, or zero if not attached
    Fiber*       fiber()         const { return fbFiber; }
    
    /// position in space (using current interpolation)
    Vector       pos()           const { return inter.pos(); }
    
    /// position (recalculated on the fly)
    Vector       posHand()       const { return fbFiber->pos(fbAbs); }
    
    /// direction of Fiber obtained by normalization
    Vector       dir()           const { return inter.dir(); }
    
    /// the direction of the Fiber at the point of attachment
    Vector       dirFiber()      const { return fbFiber->dirSegment(inter.point1()); }
    
    /// the abscissa, from the origin of the Fiber
    real         abscissa()      const { return fbAbs; }

    /// abscissa, counted from the MINUS_END
    real         abscissaFromM() const { return fbAbs - fbFiber->abscissaM(); }

    /// abscissa, counted from the PLUS_END in the reverse direction
    real         abscissaFromP() const { return fbFiber->abscissaP() - fbAbs; }

    /// abscissa, counted from the specified FiberEnd (in reversed direction for the PLUS_END)
    real         abscissaFrom(FiberEnd ref) const;
            
    /// nearest end to the current attachment point
    FiberEnd     nearestEnd() const;
    
    /// distance to the closest fiber tip
    real         distanceToEnd(FiberEnd) const;

    /// true if abscissa is below abscissaP
    bool         belowP()        const { return fbFiber->belowP(fbAbs); }
    
    /// true if abscissa is above abscissaM
    bool         aboveM()        const { return fbFiber->aboveM(fbAbs); }
    
    /// true if abscissa is within the fiber boundaries
    bool         betweenMP()     const { return fbFiber->betweenMP(fbAbs); }
    
    //--------------------------------------------------------------------------
    
    /// read from file
    void         read(Inputter&, Simul&);
    
    /// write to file
    void         write(Outputter&) const;
 
    /// Human friendly ouput
    void         print(std::ostream&) const;
    
    //--------------------------------------------------------------------------
    
    /// check that fbAbs is within Fiber::abscissaM() and Fiber::abscissaP()
    void         checkAbscissa() const;
    
    /// check validity of the interpolation (debuging purposes)
    int          bad() const;
};

/// output operator for debugging purpose
std::ostream& operator << (std::ostream&, FiberSite const&);


#endif

