#ifndef _REG_REMAPPING_H_
#define _REG_REMAPPING_H_

struct RegisterRemappingRequest{
    uint32_t dst_areg;
    uint32_t src_areg0;
    uint32_t src_areg1;
    bool GC_needed;

    Reset(){
        dst_areg = 0;
        src_areg0 = 0;
        src_areg1 = 0;
        GC_needed = 0;
    }
    RegisterRemappingRequest(){
        Reset();
    }
}

struct RegisterRemappingResult{
    uint32_t dst_preg;
    uint32_t src_preg0;
    uint32_t src_preg1;
    uint32_t GC;

    Reset(){
        dst_preg = 0;
        src_preg0 = 0;
        src_preg1 = 0;
        GC = 0;
    }
    RegisterRemappingResult(){
        Reset();
    }
}

struct RestoreRATRequest{
    uint32_t GC;

    Reset(){
        GC = 0;
    }
    RestoreRATRequest(){
        Reset();
    }
}

#endif /* _REG_REMAPPING_H_ */