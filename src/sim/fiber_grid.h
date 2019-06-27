// Cytosim was created by Francois Nedelec. Copyright 2007-2017 EMBL.

#ifndef FIBER_GRID_H
#define FIBER_GRID_H

#include "dim.h"
#include "vector.h"
#include "array.h"
#include "grid.h"
//#include <vector>

#ifdef DISPLAY
#include "grid_display.h"
#endif


class Simul;
class PropertyList;
class FiberSegment;
class FiberSet;
class Modulo;
class Space;
class Fiber;
class HandProp;
class Hand;
class Node;


/// Divide-and-Conquer method to find all FiberSegment located near a given point in space
/**
A divide-and-conquer algorithm is used to find all segments of fibers close to a given point:
 
 -# It uses a grid 'fGrid' covering the space, initialized by setGrid().
    After initialization, each cell of the grid has an empty SegmentList (a list of FiberSegment*).
 -# clear() resets all lists on the grid
 -# paintGrid() distributes the segments specified in the arguments to the cell-associated SegmentList.
    One of the argument specifies a maximum distance to be queried (`max_range`).
    After the distribution, tryToAttach() is able to find any segment
    located at a distance `max_range` or less from any given point, in linear time.
 -# The function tryToAttach(X, ...) finds the cell on fGrid that contain `X`. 
    The associated SegmentList will then contains all the segments located at distance `max_range` or less from `X`. 
    tryToAttach() calls a function distanceSqr() sequentially for all the segments in this list,
    to calculate the exact Euclidian distance. 
    Finally, using a random number it tests the probability of attachment for the Hand given as argument.
 .
 
 @todo we could call paintGrid() only if the objects have moved by a certain threshold.
 This would work if we also extend the painted area around the rod, by the same threshold.
 we must also redo the paintGrid() when MT points are added or removed.
 
 Such algortihm should lead to large CPU gain, if calling clear() or paintGrid() is limiting,
 which is the case in particular in 3D, because the number of grid-cells is large.
*/

class FiberGrid 
{
public:
    
    /// type for a list of FiberSegment
    typedef Array<FiberSegment> SegmentList;
    //typedef std::vector<FiberSegment> SegmentList;

    /// type of grid
    typedef Grid<SegmentList, DIM> grid_type;
    
    /// type of index
    typedef grid_type::index_t index_t;
    
private:
    
    ///the maximum binding distance that can handled by the grid
    real  gridRange;
    
    ///grid for divide-and-conquer strategies:
    grid_type fGrid;
    
public:
    
    /// constructor
    FiberGrid()  { gridRange = -1; }
    
    /// set binding range
    void         setRange(real s) { gridRange = s; }
    
    /// return range
    real         range() const { return gridRange; }
    
    /// number of cells in grid
    index_t      nbCells() const { return fGrid.nbCells(); }

    /// set a grid to cover the specified Space with cells of width `max_step` at most
    unsigned     setGrid(Space const*, real max_step);
    
    /// allocate memory for the grid, with the dimensions set by setGrid()
    void         createCells();
    
    /// true if the grid was initialized by calling setGrid()
    size_t       hasGrid() const;
    
    /// register the Fiber segments on the grid cells
    void         paintGrid(const Fiber * first, const Fiber * last);
    
    /// given a position, find nearby Fiber segments and test attachement of the provided Hand
    void         tryToAttach(Vector const&, Hand&) const;
    
    /// return all fiber segments located at a distance D or less from P, except those belonging to `exclude`
    SegmentList  nearbySegments(Vector const& P, real D, Fiber * exclude = nullptr) const;

    /// Among the segments closer than gridRange, return the closest one
    FiberSegment closestSegment(Vector const&) const;
    
    ///test the results of tryToAttach(), at a particular position
    void         testAttach(FILE *, Vector place, FiberSet const&, HandProp const*) const;
    
#ifdef DISPLAY
    void draw() const
    {
        glPushAttrib(GL_LIGHTING_BIT);
        glDisable(GL_LIGHTING);
        glColor4f(0, 1, 1, 1);
        glLineWidth(0.5);
        drawEdges(fGrid);
        glPopAttrib();
    }
#endif
 };


#endif
