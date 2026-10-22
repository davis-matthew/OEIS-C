/*
 * A333708  Number of ways to write n as the sum of two distinct positive
 *          integers with the same number of distinct prime factors.
 * https://oeis.org/A333708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} [omega(i)=omega(n-i)]
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_distinct_primes.h"

/* Computes the OEIS sequence A333708:  
   number of ways to write n as a sum of two distinct positive integers
   having the same number of distinct prime factors. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    seq_t result = (seq_t)0;
    idx_t limit = (n - 1) / 2;

    for (idx_t i = 1; i <= limit; ++i)
    {
        int w1 = count_distinct_primes((uint64_t)i);
        int w2 = count_distinct_primes((uint64_t)(n - i));
        if (w1 == w2)
            result += (seq_t)1;
    }
    return result;
}
