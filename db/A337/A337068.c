/*
 * A337068  a(n) is the least number of repetitions such that the result of
 *          the repeated execution of the division f <- f/n started at f=1
 *          produces 0, when the division is performed using Commodore BASIC.
 * https://oeis.org/A337068
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = floor(128 / log2(n)) + 1
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n < 2u)
        return 0;           /* undefined for n < 2; zero is a placeholder */

    /* Compute the exponent threshold with high‑precision floating point. */
    long double ln = log2l((long double)n);
    /* Add a tiny epsilon to guard against round‑off when n is an exact power of 2. */
    long double kf = floorl(128.0L / ln + 1e-12L) + 1.0L;
    return (seq_t)(uint64_t)kf;
}
