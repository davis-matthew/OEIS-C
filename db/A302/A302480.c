/*
 * A302480  Number of partitions of n into two parts with the smaller part
 *          nonprime and the larger part prime.
 * https://oeis.org/A302480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊n/2⌋} (1 - A010051(i)) * A010051(n-i)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts partitions of n into two parts: the smaller part is
   nonprime, the larger part is prime. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 1; i <= n / 2; ++i) {
        if (!is_prime(i) && is_prime(n - i))
            res++;
    }
    return res;
}
