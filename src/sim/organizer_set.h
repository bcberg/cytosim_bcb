// Cytosim was created by Francois Nedelec. Copyright 2007-2017 EMBL.

#ifndef ORGANIZER_SET_H
#define ORGANIZER_SET_H

#include "object_set.h"
#include "organizer.h"

class Simul;
class Aster;

/// a list of Organizer (Aster, Nucleus, Bundle)
class OrganizerSet : public ObjectSet
{
public:
    
    ///creator
    OrganizerSet(Simul& s) : ObjectSet(s) {}
    
    //--------------------------
    
    /// identifies the class
    std::string title() const { return "organizer"; }
    
    /// create a new property of category `cat` for a class `name`
    Property *  newProperty(const std::string& cat, const std::string& name, Glossary&) const;
    
    /// create objects of class `name`, given the options provided in `opt`
    ObjectList  newObjects(const std::string& name, Glossary& opt);
    
    /// create a new object (used for reading trajectory file)
    Object *    newObjectT(ObjectTag, unsigned);
    
    //--------------------------

    /// register Organizer
    void        add(Object *);
    
    /// first Organizer
    Organizer * first() const
    {
        return static_cast<Organizer*>(nodes.front());
    }
    
    /// find object with given ID
    Organizer * findID(ObjectID n) const
    {
        return static_cast<Organizer*>(inventory.get(n));
    }
    
    /// find Aster with given ID
    Aster *     findAster(ObjectID n) const;
    
    /// find first Organizer containing given Mecable
    Organizer*  findOrganizer(Mecable const*) const;
    
    /// modulo the position (periodic boundary conditions)
    void        foldPosition(Modulo const* s) const;

    /// Monte-Carlo simulation step for every Object
    void        step();

    /// print a summary of the content (nb of objects, class)
    void        report(std::ostream&) const;
};


#endif


