/*
 * A337909  Distinct terms of A080079 in the order in which they appear.
 * https://oeis.org/A337909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; for n>2, a(n)=3*2^floor_log2(n-1)-(n-1)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    idx_t y = n - 1;
    idx_t h = floor_log2_uint64(y);     /* ⌊log₂(n‑1)⌋ */
    idx_t p = (idx_t)1 << h;            /* 2^h */
    idx_t pow2 = p << 1;                /* 2^(h+1) */
    idx_t diff = y - p;                 /* (n‑1) – 2^h */
    return (seq_t)(pow2 - diff);        /* 2^(h+1) – [(n‑1)–2^h] */
}
