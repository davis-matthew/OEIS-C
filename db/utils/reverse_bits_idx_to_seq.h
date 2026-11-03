/* reverse_bits_idx_to_seq: n with its binary digits reversed.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_REVERSE_BITS_IDX_TO_SEQ_H
#define OEISC_UTILS_REVERSE_BITS_IDX_TO_SEQ_H
#include <stdint.h>

static seq_t reverse_bits_idx_to_seq(idx_t n) {
    seq_t rev = 0;
    while (n > 0) {
        rev = (rev << 1) | (seq_t)(n & 1);
        n >>= 1;
    }
    return rev;
}

#endif
