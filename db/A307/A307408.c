/*
 * A307408  a(n) = (A001222(n) - 1)*A001221(n) + 2.
 * https://oeis.org/A307408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Ω(n) - 1) * ω(n) + 2
 * Range:     u8 n <= 9927, u16 n <= 9927, u32 n <= 9927, u64 n <= 9927,
 *            u128 n <= 9927, big any n
 * Helpers:   bigomega, count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ω(n) = A001222(n) (number of prime factors of n, counted with multiplicity) */
/* ω(n) = A001221(n) (number of distinct prime factors of n) */

#include "../utils/bigomega.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    seq_t omega = bigomega(n);                 /* Ω(n) */
    seq_t distinct = (seq_t)count_distinct_primes((uint64_t)n); /* ω(n) */
    seq_t diff = (omega > 0) ? omega - 1 : 0;  /* avoid unsigned underflow for n=1 */
    return diff * distinct + 2;
}
