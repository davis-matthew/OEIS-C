/*
 * A354870  Number of nonprime squarefree divisors of n.
 * https://oeis.org/A354870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^ω(n) - ω(n) where ω(n) is the number of distinct prime
 *            factors of n. A(n) counts the square‑free divisors of n that are
 *            not primes, i.e. 1 and any composite square‑free divisor
 * Range:     u8 n <= 9484, u16 n <= 9484, u32 n <= 9484, u64 n <= 9484,
 *            u128 n <= 9484, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* defined only for n >= 1 */

    /* distinct prime factor count ω(n) */
    int omega = count_distinct_primes((uint64_t)n);

    /* compute 2^ω(n) using only seq_t arithmetic */
    seq_t sqfree = (seq_t)1;
    for (int i = 0; i < omega; ++i)
        sqfree = sqfree * (seq_t)2;

    /* subtract the ω(n) primes themselves */
    return sqfree - (seq_t)omega;
}
