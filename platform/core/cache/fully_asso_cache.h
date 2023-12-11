#ifndef _FULLY_ASSO_CACHE_IF_H_
#define _FULLY_ASSO_CACHE_IF_H_

// this is a pesudo LRU
#include <stdint.h>
#include "cache_if.h"

template<uint32_t BYTES_PER_CACHE_LINE, uint32_t CACHE_LINE_NUM>
class FullAssoCache : public CacheIf{
    public:
    FullAssoCache();
    virtual void Reset();
    virtual void Flush();
    virtual CACHE_RESULT Read(const uint32_t tag, const uint32_t index, uint8_t* buffer, const uint32_t size);
    virtual CACHE_RESULT Write(const uint32_t tag, const uint32_t index, uint8_t* buffer, const uint32_t size);
    virtual ~FullAssoCache();

    private:
    void Update();
    uint32_t GetTag(const uint32_t address);

    uint8_t data_[CACHE_LINE_NUM][BYTES_PER_CACHE_LINE];
    uint32_t tag_[CACHE_LINE_NUM];
    bool valid_[CACHE_LINE_NUM];
    bool dirty_[CACHE_LINE_NUM];

    
};


#endif /* _FULLY_ASSO_CACHE_IF_H_ */