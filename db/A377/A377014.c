/*
 * A377014  a(n) is the number of primes p such that p - 6, p + 6 and 2*n - p
 *          are also primes.
 * https://oeis.org/A377014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes p such that p-6, p+6 and 2*n-p are all
 *            prime
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Avoid overflow of 2*n. For huge n we simply return 0. */
    if (n > (UINT64_MAX / 2)) return 0;

    idx_t upper = n * 2;        /* 2*n */
    if (upper <= 8)            /* smallest admissible p is 7 */
        return 0;

    idx_t max_p = upper - 2;    /* p <= 2*n-2 */
    seq_t cnt = 0;

    for (idx_t p = 7; p <= max_p; ++p)
    {
        if (!is_prime(p))              continue;
        if (!is_prime(p - 6))          continue;
        if (!is_prime(p + 6))          continue;

        idx_t q = upper - p; /* 2*n - p */
        if (q < 2)             continue;
        if (!is_prime(q))      continue;

        ++cnt;
    }
    return cnt;
}
