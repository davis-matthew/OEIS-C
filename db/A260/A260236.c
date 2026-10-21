/*
 * A260236  Number of prime factors, with multiplicity, of the n-th hexagonal
 *          number (A000384).
 * https://oeis.org/A260236
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(A000384(n))
 * Range:     u8 n <= 11, u16 n <= 430, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n) = number of prime factors (with multiplicity) of the n-th hexagonal number */
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    if (n < 2)                  /* sequence offset is 2 */
        return 0;

    /* Hexagonal number H_n = n * (2*n - 1) */
    seq_t hn = (seq_t)n * ((seq_t)2 * (seq_t)n - 1);

    return count_prime_factors(hn);
}
