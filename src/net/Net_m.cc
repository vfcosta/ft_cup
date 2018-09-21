//
// Generated file, do not edit! Created by opp_msgc 4.0 from Net.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "Net_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(Net);

Net::Net(const char *name, int kind) : NetwPkt(name,kind)
{
    this->initiator_var = 0;
}

Net::Net(const Net& other) : NetwPkt()
{
    setName(other.getName());
    operator=(other);
}

Net::~Net()
{
}

Net& Net::operator=(const Net& other)
{
    if (this==&other) return *this;
    NetwPkt::operator=(other);
    this->initiator_var = other.initiator_var;
    this->known_var = other.known_var;
    this->dest_var = other.dest_var;
    return *this;
}

void Net::parsimPack(cCommBuffer *b)
{
    NetwPkt::parsimPack(b);
    doPacking(b,this->initiator_var);
    doPacking(b,this->known_var);
    doPacking(b,this->dest_var);
}

void Net::parsimUnpack(cCommBuffer *b)
{
    NetwPkt::parsimUnpack(b);
    doUnpacking(b,this->initiator_var);
    doUnpacking(b,this->known_var);
    doUnpacking(b,this->dest_var);
}

int Net::getInitiator() const
{
    return initiator_var;
}

void Net::setInitiator(int initiator_var)
{
    this->initiator_var = initiator_var;
}

NeighborDetector& Net::getKnown()
{
    return known_var;
}

void Net::setKnown(const NeighborDetector& known_var)
{
    this->known_var = known_var;
}

NeighborDetector& Net::getDest()
{
    return dest_var;
}

void Net::setDest(const NeighborDetector& dest_var)
{
    this->dest_var = dest_var;
}

class NetDescriptor : public cClassDescriptor
{
  public:
    NetDescriptor();
    virtual ~NetDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual bool getFieldAsString(void *object, int field, int i, char *resultbuf, int bufsize) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(NetDescriptor);

NetDescriptor::NetDescriptor() : cClassDescriptor("Net", "NetwPkt")
{
}

NetDescriptor::~NetDescriptor()
{
}

bool NetDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Net *>(obj)!=NULL;
}

const char *NetDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NetDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int NetDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return FD_ISEDITABLE;
        case 1: return FD_ISCOMPOUND;
        case 2: return FD_ISCOMPOUND;
        default: return 0;
    }
}

const char *NetDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return "initiator";
        case 1: return "known";
        case 2: return "dest";
        default: return NULL;
    }
}

const char *NetDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return "int";
        case 1: return "NeighborDetector";
        case 2: return "NeighborDetector";
        default: return NULL;
    }
}

const char *NetDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int NetDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Net *pp = (Net *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

bool NetDescriptor::getFieldAsString(void *object, int field, int i, char *resultbuf, int bufsize) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i,resultbuf,bufsize);
        field -= basedesc->getFieldCount(object);
    }
    Net *pp = (Net *)object; (void)pp;
    switch (field) {
        case 0: long2string(pp->getInitiator(),resultbuf,bufsize); return true;
        case 1: {std::stringstream out; out << pp->getKnown(); opp_strprettytrunc(resultbuf,out.str().c_str(),bufsize-1); return true;}
        case 2: {std::stringstream out; out << pp->getDest(); opp_strprettytrunc(resultbuf,out.str().c_str(),bufsize-1); return true;}
        default: return false;
    }
}

bool NetDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Net *pp = (Net *)object; (void)pp;
    switch (field) {
        case 0: pp->setInitiator(string2long(value)); return true;
        default: return false;
    }
}

const char *NetDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 1: return "NeighborDetector"; break;
        case 2: return "NeighborDetector"; break;
        default: return NULL;
    }
}

void *NetDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Net *pp = (Net *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getKnown()); break;
        case 2: return (void *)(&pp->getDest()); break;
        default: return NULL;
    }
}


