// Cytosim was created by Francois Nedelec. Copyright 2007-2017 EMBL.

#ifndef CLASSIC_FIBER_H
#define CLASSIC_FIBER_H

#include "sim.h"
#include "vector.h"
#include "node_list.h"
#include "common.h"
#include "fiber.h"


class ClassicFiberProp;


/// A Fiber with a standard two-state model of dynamic instability at the PLUS_END
/**
 This implements the 'classical' two-state model of dynamic instability:
 - a growing state, affected by force,
 - stochastic catastrophes, affected by force,
 - a shrinking state characterized by a constant speed,
 - stochastic rescues.
 .
 
 Only the PLUS_END can grow/shrink and does not have any granularity.
 The length is incremented at each time step by <em> time_step * tip_speed </em>.
 
 The speed of the tip `tip_speed` is a fraction of `prop->growing_speed`,
 because the growth speed is reduced under antagonistic force by an exponential factor:
 <em>
 <b>Measurement of the Force-Velocity Relation for Growing Microtubules</b>\n
 Marileen Dogterom and Bernard Yurke\n
 Science Vol 278 pp 856-860; 1997\n
 http://dx.doi.org/10.1126/science.278.5339.856 \n
 http://www.sciencemag.org/content/278/5339/856.abstract
 </em>
 
 ...and this increases the catastrophe rate:\n
 <em>
 <b>Dynamic instability of MTs is regulated by force</b>\n
 M.Janson, M. de Dood, M. Dogterom.\n
 Journal of Cell Biology Vol 161, Nb 6, 2003\n
 Figure 2 C\n
 </em>
 http://dx.doi.org/10.1083/jcb.200301147 \n
 http://jcb.rupress.org/content/161/6/1029
 The growth speed is linearly proportional to free tubulin concentration.

 See the @ref ClassicFiberPar.

 This class is not fully tested (17. Feb 2011).
 @ingroup FiberGroup
 */
class ClassicFiber : public Fiber
{   
private:
    
    /// state of MINUS_END
    AssemblyState  mStateM;
    
    /// length increment at MINUS_END during last time-step
    real           mGrowthM;

    /// state of PLUS_END
    AssemblyState  mStateP;
    
    /// length increment at PLUS_END during last time-step
    real           mGrowthP;
     
public:
    
    /// the Property of this object
    ClassicFiberProp const* prop;
  
    /// constructor
    ClassicFiber(ClassicFiberProp const*);

    /// destructor
    virtual    ~ClassicFiber();
        
    //--------------------------------------------------------------------------
    
    /// return assembly/disassembly state of MINUS_END
    unsigned    dynamicStateM() const { return mStateM; }

    /// return assembly/disassembly state of PLUS_END
    unsigned    dynamicStateP() const { return mStateP; }

    
    /// length increment at MINUS_END during last time-step
    real        freshAssemblyM() const { return mGrowthM; }
    
    /// length increment at PLUS_END during last time-step
    real        freshAssemblyP() const { return mGrowthP; }

    
    /// change state of MINUS_END
    void        setDynamicStateM(unsigned s);
    
    /// change state of PLUS_END
    void        setDynamicStateP(unsigned s);

    /// monte-carlo step
    void        step();
    
    //--------------------------------------------------------------------------
    
    /// write to Outputter
    void        write(Outputter&) const;

    /// read from Inputter
    void        read(Inputter&, Simul&, ObjectTag);
    
};


#endif
