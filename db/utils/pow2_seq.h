/* pow2_seq: 2^n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POW2_SEQ_H
#define OEISC_UTILS_POW2_SEQ_H
#include <stdint.h>

static seq_t pow2_seq(seq_t k) {
    if (k == (seq_t)0) return (seq_t)1;
    
    seq_t result = (seq_t)1;
    seq_t base = (seq_t)2;
    seq_t exp = k;
    
    while (exp > (seq_t)0) {
        if (exp & (seq_t)1) {
            result = result * base;
        }
        base = base * base;
        exp = exp >> 1;
    }
    
    return result;
}

#endif
