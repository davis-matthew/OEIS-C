/* ctz_seq_to_idx: the number of trailing zero bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_CTZ_SEQ_TO_IDX_H
#define OEISC_UTILS_CTZ_SEQ_TO_IDX_H
#include <stdint.h>

static idx_t ctz_seq_to_idx(seq_t v)
{
    idx_t cnt = 0;
    while ((v & (seq_t)1) == 0 && v != 0) {
        v >>= 1;
        cnt++;
    }
    return cnt;
}

#endif
