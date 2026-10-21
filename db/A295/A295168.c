/*
 * A295168  Chromatic invariant of the 2n-crossed prism graph.
 * https://oeis.org/A295168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - 2^(n - 1)*(3*n + 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: exponentiation by squaring for seq_t base, idx_t exponent */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* sequence is defined for n >= 1 */

    /* compute 5^n */
    seq_t pow5 = pow_seq((seq_t)5, n);

    /* compute 2^(n-1) */
    seq_t pow2 = pow_seq((seq_t)2, n - 1);

    /* compute 3*n + 1 */
    seq_t mult = (seq_t)3 * (seq_t)n + (seq_t)1;

    /* a(n) = pow5 - pow2 * mult */
    return pow5 - pow2 * mult;
}
