/*
 * A291703  Number of connected dominating sets in the complete tripartite
 *          graph K_{n,n,n}.
 * https://oeis.org/A291703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 - 3*2^n + 8^n (for n > 1), a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"   /* provides seq_pow(seq_t, idx_t) */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)7;
    }

    /* Compute powers of 2 and 8 using the provided big‑integer power routine. */
    seq_t pow2n = seq_pow((seq_t)2, n);
    seq_t pow8n = seq_pow((seq_t)8, n);

    /* a(n) = 8^n + 2 - 3*2^n */
    seq_t result = pow8n;
    result -= (seq_t)3 * pow2n;
    result += (seq_t)2;

    return result;
}
