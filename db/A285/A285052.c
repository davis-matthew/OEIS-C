/*
 * A285052  Number of idempotent equivalence classes for multiplication in Zn.
 * https://oeis.org/A285052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^A001221(n) where A001221(n) is the number of distinct
 *            prime factors of n
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   count_distinct_primes, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_distinct_primes.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* For n >= 1. count_distinct_primes(1) returns 0, so a(1) = 4^0 = 1. */
    int w = count_distinct_primes((uint64_t)n);   /* ω(n) */
    return pow_seq((seq_t)4, w);                  /* 4^ω(n) */
}
