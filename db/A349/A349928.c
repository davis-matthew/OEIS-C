/*
 * A349928  a(n) = Sum_{k=0..n} (k+n)^k.
 * https://oeis.org/A349928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+n)^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for bool, not strictly needed but harmless */

/* Helper: exponentiation by squaring for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > (idx_t)0)
    {
        if (exp & (idx_t)1)
            result = result * base;
        base = base * base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    seq_t N   = (seq_t)n;
    for (idx_t k = (idx_t)0; k <= n; ++k)
    {
        seq_t term;
        if (k == (idx_t)0)
            term = (seq_t)1; /* (n+0)^0 = 1, even when n = 0 */
        else
            term = pow_seq(N + (seq_t)k, k);
        sum += term;
    }
    return sum;
}
