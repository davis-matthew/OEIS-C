/*
 * A323155  a(n) = Product_{d|n, d-1 is prime} (d-1)^(1+A286561(n,d-1)), where
 *          A286561(n,k) gives the k-valuation of n (for k > 1).
 * https://oeis.org/A323155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d-1 is prime} (d-1)^(1 + A286561(n,d-1))
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 71, u64 n <= 359,
 *            u128 n <= 2159, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t res = 1;
    if (n < 2) {
        /* For n = 1, the product is empty, so the value is 1. */
        return 1;
    }

    /* Iterate over all divisors d of n. */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0)
            continue;

        /* Process divisor d (if d > 1). */
        if (d > 1) {
            idx_t p = d - 1;
            if (p >= 2 && is_prime(p)) {
                /* Compute the p‑valuation of n. */
                idx_t tmp = n;
                idx_t v = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++v;
                }
                /* Compute (p)^(1+v). */
                seq_t pow = 1;
                for (idx_t i = 0; i < v + 1; ++i)
                    pow *= (seq_t)p;
                res *= pow;
            }
        }

        /* Process the paired divisor d2 = n/d, omitted if it equals d. */
        idx_t d2 = n / d;
        if (d2 == d)
            continue;          /* square root case */

        if (d2 > 1) {
            idx_t p = d2 - 1;
            if (p >= 2 && is_prime(p)) {
                idx_t tmp = n;
                idx_t v = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++v;
                }
                seq_t pow = 1;
                for (idx_t i = 0; i < v + 1; ++i)
                    pow *= (seq_t)p;
                res *= pow;
            }
        }
    }

    return res;
}
