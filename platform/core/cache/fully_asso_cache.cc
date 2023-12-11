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
    memset(plru_bits_,0,sizeof(bool)*(CACHE_LINE_NUM-1));
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
            Update(i,0,plru_bits_);
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
            Update(i,0,plru_bits_);
            return CACHE_HIT;
        }
    }

    // miss
    return CACHE_MISS;
}

template<uint32_t BYTES_PER_CACHE_LINE, uint32_t CACHE_LINE_NUM>
void FullAssoCache<BYTES_PER_CACHE_LINE, CACHE_LINE_NUM>::SwapCacheLine(const uint32_t in_tag, const uint8_t* in_buffer, uint32_t& out_tag, uint8_t* out_buffer, bool& is_dirty){
    // calc swaped line
    uint32_t oldest_line = VictimLine();
    out_tag = tag_[oldest_line];
    is_dirty = dirty_[oldest_line] && valid_[oldest_line];
    dirty_[oldest_line] = false;
    valid_[oldest_line] = true;
    for(unsigned i = 0; i < BYTES_PER_CACHE_LINE;i++){
        out_buffer[i] = data_[i];
        data_[i] = in_buffer[i];
    }
    
    Update(oldest_line,0,plru_bits_);
}

template<uint32_t BYTES_PER_CACHE_LINE, uint32_t CACHE_LINE_NUM>
void FullAssoCache<BYTES_PER_CACHE_LINE, CACHE_LINE_NUM>::Update(uint32_t access_way, uint32_t level ,bool* plru_bits){
    // p-lru
    // 从后面开始倒推
    uint32_t access_way_bit = access_way / 2;
    plru_bits[access_way_bit] = !plru_bits[access_way_bit];
    // 该level总占的bit个数就是 cache_line_num/2^level
    uint32_t plru_bit_stride = CACHE_LINE_NUM / (1 << (level+1));
    if(plru_bit_stride == 1){
        // 最后一个level更新完毕，结束
        return;
    }
    // 更新下一个level
    Update(access_way_bit,level+1,plru_bits+plru_bit_stride);
}

template<uint32_t BYTES_PER_CACHE_LINE, uint32_t CACHE_LINE_NUM>
uint32_t FullAssoCache<BYTES_PER_CACHE_LINE, CACHE_LINE_NUM>::VictimLine(uint32_t level,uint32_t offset){
    // 计算当前指针指向的位置
    // 从最后一个level出发，即从开头出发
    uint32_t total_level = log2(CACHE_LINE_NUM);
    uint32_t actually_level = total_level - 1 - level;
    
    uint32_t level_stride = CACHE_LINE_NUM / (1 << level);
    uint32_t level_start_idx = CACHE_LINE_NUM - CACHE_LINE_NUM / (1 << level);

    // 查找当前bit是否是正，如果为0，则取下部分，如果是1，取上部分
    uint32_t next_level_offset = offset*2 + plru_bits_[level_start_idx + offset]?
                            1:0;
    // 计算当前level每个block有几个line
    uint32_t per_line_each_block = CACHE_LINE_NUM/actually_level;
    uint32_t block_sel = plru_bits_[level_start_idx + offset]?1:0;
    // 如果当前是最后一个level
    if(per_line_each_block == 1){
        return block_sel;
    }
    // 递归查找下一个level
    return block_sel*per_line_each_block + VictimLine(level+1,next_level_offset);
}

