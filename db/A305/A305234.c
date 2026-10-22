/*
 * A305234  Number of successors of n having the same value of A001221 as n.
 * https://oeis.org/A305234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A305234: for n ≥ 1 count the number of successive integers > n
   that have the same number of distinct prime divisors as n 
   (i.e. the same value of A001221). */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    int w = count_distinct_primes((uint64_t)n);
    seq_t cnt = 0;

    for (idx_t m = n + 1; ; ++m)
    {
        if (count_distinct_primes((uint64_t)m) != w)
            break;
        ++cnt;
    }

    return cnt;
}
