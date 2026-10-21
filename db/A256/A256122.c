/*
 * A256122  Number of iterations needed to reach 0 or 1 under the map n->
 *          n-sopf(n), where sopf(n) is the sum of the distinct primes
 *          dividing n (A008472).
 * https://oeis.org/A256122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A256122: a(n) = number of iterations needed to reach 0 or 1
 * under the map n -> n - sopf(n), where sopf(n) is the sum of the
 * distinct primes dividing n.  The implementation uses the helper
 * function _smallest_prime_factor to obtain prime factors efficiently. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"

/* Compute the sum of distinct prime factors of a 64‑bit integer. */
static uint64_t sopf_uint64(uint64_t n)
{
    if (n <= 1)
        return 0;
    uint64_t sum = 0;
    while (n > 1) {
        uint64_t p = _smallest_prime_factor(n);
        sum += p;
        /* skip repeated occurrences of p */
        while (n % p == 0)
            n /= p;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t cnt = 0;               /* number of iterations */
    uint64_t cur = (uint64_t)n;  /* current value – fits in 64‑bit */

    while (cur > 1) {
        cur -= sopf_uint64(cur);  /* one iteration of the map */
        ++cnt;                    /* count this iteration */
    }
    return cnt;                  /* reached 0 or 1 */
}
