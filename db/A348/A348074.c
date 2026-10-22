/*
 * A348074  a(n) = A035327(n) if n is even, 3n + 1 if n is odd.
 * https://oeis.org/A348074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n + 1 if n is odd, a(0) = 1 if n is even and n == 0,
 *            a(2k) = 2^(m+1) - (2*(k - 2^m) + 1) if n = 2k > 0 where m =
 *            floor(log2(k))
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int floor_log2_u64(unsigned long long x)
{
    /* x > 0; returns floor(log2(x)) */
    int r = 0;
    while (x >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    if ((n & 1UL) != 0) /* odd n */
    {
        return (seq_t)3 * n + 1;
    }

    /* even n */
    unsigned long long k = n >> 1; /* k = n/2 */

    if (k == 0) /* n == 0 */
        return (seq_t)1;

    int m = floor_log2_u64(k);                     /* m = floor(log2(k)) */
    seq_t base = ((seq_t)1) << (m + 1);            /* 2^(m+1) */
    seq_t two_k_minus = ((seq_t)2) * ((seq_t)k - ((seq_t)1 << m));
    seq_t diff = two_k_minus + 1;                  /* 2*(k-2^m)+1 */
    return base - diff;
}
