/*
 * A261695  First differences of A256534.
 * https://oeis.org/A261695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 0, n = 0; for n >= 1, a(n) = 12 + 24 * ((n-1) -
 *            2^floor(log2(n-1))) which equals 4 * A241717(n-1) with
 *            A241717(k) = 6*(k-2^floor(log2 k)) + 3 for k>=1. A241717(0) = 1
 * Range:     u8 n <= 27, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation assumes 0 <= n. */

#include <stddef.h>
#include <stdint.h>

/* Compute floor(log2(x)) for x >= 1. */
static inline idx_t floor_log2_u64(idx_t x)
{
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t k    = n - 1;                      /* k = n-1 >= 0 */
    if (k == 0) return 4;                   /* A241717(0)=1 => 4 */
    idx_t h    = floor_log2_u64(k);         /* h = floor(log2(k)) */
    idx_t pow2 = 1ULL << h;                 /* 2^h <= k */
    idx_t offset = k - pow2;                /* offset in [0, 2^h-1] */
    /* a(n) = 12 + 24 * offset */
    seq_t result = (seq_t)12 + (seq_t)24 * (seq_t)offset;
    return result;
}
