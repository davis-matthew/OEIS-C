/*
 * A284722  (2n+1-A284721(n))/2.
 * https://oeis.org/A284722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2n+1 - p(n))/2, where p(n) is the smallest odd prime *
 *            that does NOT divide (2n+1)
 * Range:     i8 n <= 63, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Compute 2n+1 as seq_t to avoid overflow. */
    seq_t h = (seq_t)n * 2 + 1;

    /* List of initial odd primes. Sufficient for all n up to 10 000. */
    static const seq_t primes[] = {3,5,7,11,13,17,19,23,29,31,
                                   37,41,43,47,53,59,61,67,71,73,
                                   79,83,89,97,101,103,107,109,113,127};

    /* Find the smallest odd prime that does not divide h. */
    for (size_t i = 0; i < sizeof(primes)/sizeof(primes[0]); ++i)
    {
        seq_t p = primes[i];
        if (h % p != 0)
            return (h - p) / 2;
    }

    /* Fallback – should never be reached. */
    return h / 2;
}
