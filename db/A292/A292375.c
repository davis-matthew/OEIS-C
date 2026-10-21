/*
 * A292375  a(1) = 1, and for n > 1, a(n) = a(A252463(n)) + [n == 1 (mod 4)].
 * https://oeis.org/A292375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = a(A252463(n)) + [n == 1 (mod 4)] for n > 1
 *            where A252463(n) = n/2 if n is even, otherwise each odd prime
 *            factor is replaced by the previous odd prime (3 → 2, 5 → 3, 7 →
 *            5, …)
 * Range:     u8 n <= 1545, u16 n <= 9891, u32 n <= 9891, u64 n <= 9891,
 *            u128 n <= 9891, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Detect whether x is a prime (assumes x fits in seq_t).  */
#include "../utils/is_prime_small.h"

/* Return the previous odd prime of an odd prime p.  For p == 3 the previous
   odd prime is 2; otherwise scan downward for the next smaller odd prime. */
static idx_t prev_odd_prime(idx_t p)
{
    if (p == 3) return 2;

    uint64_t candidate = p - 2;
    while (candidate >= 3) {
        if (is_prime_small((seq_t)candidate)) return candidate;
        candidate -= 2;
    }
    return 2;   /* Fallback – never reached for valid input. */
}

/* Apply the A252463 map to n. */
static idx_t map_A252463(idx_t n)
{
    if ((n & 1u) == 0)   /* even */
        return n >> 1;

    uint64_t m   = (uint64_t)n;
    uint64_t res = 1;

    /* Factor out all odd prime powers. */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            uint64_t prev = (p == 3) ? 2 : prev_odd_prime(p);
            for (unsigned i = 0; i < exp; ++i)
                res *= prev;
        }
    }

    /* If a prime factor larger than sqrt(m) remains. */
    if (m > 1) {
        uint64_t prev = (m == 3) ? 2 : prev_odd_prime(m);
        res *= prev;
    }

    return res;
}

/* Sequence a(n) as defined in OEIS A292375. */
seq_t a(idx_t n)
{
    idx_t   curr  = n;
    seq_t   count = 0;

    while (curr != 1) {
        if ((curr & 3u) == 1)   /* curr ≡ 1 (mod 4) */
            ++count;
        curr = map_A252463(curr);
    }

    /* Include the terminating 1 in the count. */
    return count + 1;
}
