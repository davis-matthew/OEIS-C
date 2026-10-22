/*
 * A399204  Number of permutations that give the minimum number of comparisons
 *          for sorting n elements by binary insertion.
 * https://oeis.org/A399204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = a(n-1) * ( 2^{⌊log2(n)⌋+1} - n ) for n ≥ 1
 *            (equivalently a(n) = a(n-1) * A080079(n))
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 17, u64 n <= 28, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes floor(log2(x)) for 1 ≤ x < 2^64 using a builtin. */
static inline idx_t floor_log2_u64(idx_t x)
{
    /* gcc/clang builtin; assumes x > 0.  For portability a manual loop could
       be used instead. */
    return 63 - __builtin_clzll(x);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t acc = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t msb  = floor_log2_u64(i);            /* ⌊log2(i)⌋      */
        idx_t pow2 = (idx_t)1 << (msb + 1);        /* 2^{⌊log2(i)⌋+1} */
        idx_t factor = pow2 - i;                   /* as in recurrence */
        acc = acc * (seq_t)factor;                /* multiply with small int   */
    }
    return acc;
}
