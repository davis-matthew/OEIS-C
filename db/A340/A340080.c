/*
 * A340080  a(n) = (1+A018804(n)) / gcd(n, 1+A018804(n)), where A018804(n) =
 *          Sum_{k=1..n} gcd(k, n).
 * https://oeis.org/A340080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + ∑_{k=1..n} gcd(k,n)) / gcd(n, 1 + ∑_{k=1..n}
 *            gcd(k,n))
 * Range:     u8 n <= 55, u16 n <= 3959, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   add_divisor_sum, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/add_divisor_sum.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n <= 0, return 0 for safety */

    /* Factor n using trial division */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t pcnt = 0;
    uint64_t tmp = (uint64_t)n;

    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {   /* remaining prime factor */
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* Compute S(n) = ∑_{k=1..n} gcd(k,n) = Σ_{d|n} d * φ(n/d) */
    seq_t sum = 0;
    add_divisor_sum(0, (idx_t)1, primes, exps, pcnt, n, &sum);

    /* a(n) = (1 + sum) / gcd(n, 1 + sum) */
    seq_t numerator = sum + 1;
    seq_t g = gcd(numerator, (seq_t)n);
    return numerator / g;
}
