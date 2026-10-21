/*
 * A289498  Number of primes in the interval [7n, 8n].
 * https://oeis.org/A289498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes in the integer interval [7n,8n]
 * Range:     u8 n <= 2544, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            // offset starts at 1, but guard anyway
    idx_t lo = 7 * n;
    idx_t hi = 8 * n;
    seq_t cnt = 0;
    for (idx_t p = lo; p <= hi; ++p) {
        if (is_prime(p)) ++cnt;
    }
    return cnt;
}
