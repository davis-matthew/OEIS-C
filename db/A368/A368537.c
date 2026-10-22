/*
 * A368537  a(n) = Sum_{k=1..n} binomial(k+2,3) * n^k.
 * https://oeis.org/A368537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} binomial(k+2,3) * n^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes A368537 for a given index n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sum   = 0;
    seq_t power = (seq_t)n;            /* n^1      */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* binomial(k+2,3) = k*(k+1)*(k+2)/6  */
        uint64_t bin_64 = (uint64_t)k * (k + 1) * (k + 2) / 6;
        seq_t bin = (seq_t)bin_64;

        sum += bin * power;

        if (k < n)
            power *= (seq_t)n;        /* next power: n^{k+1} */
    }
    return sum;
}
