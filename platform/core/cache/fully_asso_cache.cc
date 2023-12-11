#include "fully_asso_cache.h"
#include "cmath"

template<uint32_t BYTES_PER_CACHE_LINE, uint32_t CACHE_LINE_NUM>
FullAssoCache<BYTES_PER_CACHE_LINE, CACHE_LINE_NUM>::FullAssoCache(){
    Reset();
}

template<uint32_t BYTES_PER_CACHE_LINE, uint32_t CACHE_LINE_NUM>
void FullAssoCache<BYTES_PER_CACHE_LINE, CACHE_LINE_NUM>::Reset(){
    memset(data_,0,sizeof(uint8_t)*CACHE_LINE_NUM*BYTES_PER_CACHE_LINE);
    memset(tag_,0,sizeof(uint32_t)*CACHE_LINE_NUM);
    memset(valid_,0,sizeof(bool)*CACHE_LINE_NUM);
    memset(dirty_,0,sizeof(bool)*CACHE_LINE_NUM);
}

template<uint32_t BYTES_PER_CACHE_LINE, uint32_t CACHE_LINE_NUM>
void FullAssoCache<BYTES_PER_CACHE_LINE, CACHE_LINE_NUM>::Flush(){
     // TODO:
}

template<uint32_t BYTES_PER_CACHE_LINE, uint32_t CACHE_LINE_NUM>
CACHE_RESULT FullAssoCache<BYTES_PER_CACHE_LINE, CACHE_LINE_NUM>::Read(const uint32_t tag, const uint32_t index, uint8_t* buffer, const uint32_t size){
    // calc offset
    uint32_t offset_bits = log2(BYTES_PER_CACHE_LINE);
    uint32_t offset_mask = 1 << offset_bits - 1;
    uint32_t offset = index & offset_mask;
    if(offset + size > CACHE_LINE_NUM){
        // TODO(fatal)
        return CACHE_INVALID;
    }
    for(unsigned i = 0; i < CACHE_LINE_NUM; i++){
       if(tag_[i]==tag && valid_[i]){
            // hit
            for(unsigned j = 0; j < size;j++){
                buffer[j] = data_[i][j+offset];
            }
            return CACHE_HIT;
        }
    }

    // miss
    return CACHE_MISS;
}

template<uint32_t BYTES_PER_CACHE_LINE, uint32_t CACHE_LINE_NUM>
CACHE_RESULT FullAssoCache<BYTES_PER_CACHE_LINE, CACHE_LINE_NUM>::Write(const uint32_t tag, const uint32_t index, uint8_t* buffer, const uint32_t size){
    // calc offset
    uint32_t offset_bits = log2(BYTES_PER_CACHE_LINE);
    uint32_t offset_mask = 1 << offset_bits - 1;
    uint32_t offset = index & offset_mask;
    if(offset + size > CACHE_LINE_NUM){
        // TODO(fatal)
        return CACHE_INVALID;
    }
    for(unsigned i = 0; i < CACHE_LINE_NUM; i++){
       if(tag_[i]==tag && valid_[i]){
            // hit
            for(unsigned j = 0; j < size;j++){
                data_[i][j+offset] = buffer[j];
            }
            dirty_[i] = true;
            return CACHE_HIT;
        }
    }

    // miss
    return CACHE_MISS;
}
