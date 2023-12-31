#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

#include "../utils/Location.h"

enum EMetaType
{
    VAR_COMMON,
    VAR_POINTER,
    VAR_ARRAY_POINTER,
    VAR_UNKNOWN,
};

struct PtrCounter
{
    int count;
    EMetaType meta;
};

class Variable
{
public:
    Variable() = delete;

    Variable( const std::string &name, const DefectLocation &loc, EMetaType mt, const std::string &type = "" )
            : name(name), type(type), loc(loc), metaType(mt)
    {
    }

    inline std::string Name() const
    {
        return name;
    }

    inline std::string Type() const
    {
        return type;
    }

    inline EMetaType MetaType() const
    {
        return metaType;
    }

    inline void MetaType( EMetaType mt )
    {
        metaType = mt;
    }

    DefectLocation getLocation() const
    {
        return loc;
    }

    void setLocation( const DefectLocation &l )
    {
        loc = l;
    }

    std::string TypeSMT() const
    {
        if((type.find("float") != std::string::npos) ||
           (type.find("double") != std::string::npos))
        {
            return "Real";
        }

        return "Int";

    }

protected:
    std::string name;
    std::string type;
    DefectLocation loc;
    EMetaType metaType;
};

#endif
