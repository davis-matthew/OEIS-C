/*
 * A364156  Ceiling of the mean of the prime factors of n (with multiplicity).
 * https://oeis.org/A364156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( A123528(n) / A123529(n) ) where A123528(n) is the
 *            sum of the prime factors of n (with multiplicity) and A123529(n)
 *            is the number of prime factors of n (with multiplicity)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the sequence A364156: ceiling of the mean of the prime factors of n. */
seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n <= 1)
        return (seq_t)0;

    /* Work with a 64‑bit copy for factorisation */
    uint64_t x = (uint64_t)n;
    seq_t sum   = (seq_t)0;   /* sum of prime factors */
    seq_t count = (seq_t)0;   /* total number of prime factors (with multiplicity) */

    /* Simple trial division: O(sqrt(n)) factorisation */
    for (uint64_t p = 2; p <= x / p; ++p) {
        while (x % p == 0) {
            sum   += (seq_t)p;
            count += (seq_t)1;
            x     /= p;
        }
    }
    /* If remainder > 1, it's a prime factor itself */
    if (x > 1) {
        sum   += (seq_t)x;
        count += (seq_t)1;
    }

    /* Ceiling of the mean: (sum + count - 1) / count */
    return (sum + count - (seq_t)1) / count;
}
