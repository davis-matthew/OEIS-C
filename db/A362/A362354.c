/*
 * A362354  a(n) = 3*(n+3)^(n-1).
 * https://oeis.org/A362354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n+3)^(n-1) (with a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the offset‑0 term specially. */
    if (n == 0)
        return (seq_t)1;

    /* For n >= 1 compute 3 * (n+3)^(n-1). */
    seq_t base = (seq_t)(n + (idx_t)3);
    seq_t exp  = (seq_t)(n - (idx_t)1);

    seq_t pow_val = big_pow(base, exp);
    return pow_val * (seq_t)3;
}
