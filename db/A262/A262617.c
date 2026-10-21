/*
 * A262617  First differences of A256266.
 * https://oeis.org/A262617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = 6 * ( 2^{⌊log2(n)⌋+1} - n ) (n ≥ 1), a(0) = 0
 * Range:     u8 n <= 63, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t floor_log2_u64(idx_t v)
{
    /* assumes v > 0 */
    return 63 - __builtin_clzll(v);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* floor(log2(n)) */
    idx_t k = floor_log2_u64(n);

    /* 2^{k+1} */
    idx_t pow2k1 = ((idx_t)1) << (k + 1);

    /* base term: 2^{k+1} - n */
    idx_t base = pow2k1 - n;

    return (seq_t)6 * (seq_t)base;
}
