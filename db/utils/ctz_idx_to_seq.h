/* ctz_idx_to_seq: the number of trailing zero bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_CTZ_IDX_TO_SEQ_H
#define OEISC_UTILS_CTZ_IDX_TO_SEQ_H
#include <stdint.h>

static inline seq_t ctz_idx_to_seq(idx_t x)
{
    seq_t cnt = 0;
    if (!x) return 0;        /* undefined, but protects against overflow */
    while ((x & 1ULL) == 0)
    {
        cnt++;
        x >>= 1;
    }
    return cnt;
}

#endif
