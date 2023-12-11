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

    void SwapCacheLine(const uint32_t in_tag, const uint8_t* in_buffer, uint32_t& out_tag, uint8_t* out_buffer, bool& is_dirty);

    private:
    void Update(uint32_t access_way, uint32_t level ,bool* plru_bits);
    uint32_t VictimLine(uint32_t level=0,uint32_t offset=0);
    uint32_t GetTag(const uint32_t address);

    uint8_t data_[CACHE_LINE_NUM][BYTES_PER_CACHE_LINE];
    uint32_t tag_[CACHE_LINE_NUM];
    bool valid_[CACHE_LINE_NUM];
    bool dirty_[CACHE_LINE_NUM];

    bool plru_bits_[CACHE_LINE_NUM-1];

    
};


#endif /* _FULLY_ASSO_CACHE_IF_H_ */