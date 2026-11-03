/* ctz_seq: the number of trailing zero bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_CTZ_SEQ_H
#define OEISC_UTILS_CTZ_SEQ_H
#include <stdint.h>

/* 2‑adic valuation of a non‑negative seq_t */
static seq_t ctz_seq(seq_t x)
{
    idx_t cnt = 0;
    while (x != 0 && (x & 1) == 0)
    {
        x /= 2;
        ++cnt;
    }
    return (seq_t)cnt;
}

#endif
