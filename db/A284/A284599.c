/*
 * A284599  Sum of twin prime (A001097) divisors of n.
 * https://oeis.org/A284599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d twin prime} d
 * Range:     u8 n <= 268, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t t = (uint64_t)n;          /* work in 64‑bit arithmetic for n <= 10 000 */
    seq_t   sum = 0;

    for (uint64_t i = 2; (unsigned __int128)i * i <= t; ++i) {
        if (t % i == 0) {
            bool twin = false;
            if (is_prime(i)) {
                /* i is a twin prime iff i-2 or i+2 is prime */
                twin = is_prime(i + 2) || (i >= 3 && is_prime(i - 2));
            }
            if (twin) sum += (seq_t)i;
            while (t % i == 0) t /= i;   /* remove all powers of i */
        }
    }

    if (t > 1) {                    /* remaining prime factor */
        bool twin = false;
        if (is_prime(t)) {
            twin = is_prime(t + 2) || (t >= 3 && is_prime(t - 2));
        }
        if (twin) sum += (seq_t)t;
    }

    return sum;
}
