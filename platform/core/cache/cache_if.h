#ifndef _CACHE_IF_H_
#define _CACHE_IF_H_

#include "../module_if/module_if.h"

enum CACHE_RESULT{
    CACHE_MISS = 0,
    CACHE_HIT,
    CACHE_INVALID
};

class CacheIf:public ModuleIf{
    public:
    virtual void Reset() = 0;
    virtual void Flush() = 0;
    virtual void Read(const uint32_t address, uint8_t* buffer, const uint32_t size) = 0;
    virtual void Write(const uint32_t address, uint8_t* buffer, const uint32_t size) = 0;
};

#endif /* _CACHE_IF_H_ */