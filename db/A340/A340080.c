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
 * Validated: standard, pass, 94 terms through n = 6826 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Recursive helper to enumerate all divisors of n and accumulate
   the sum Σ_{d|n} d * φ(n/d) in *sum. */
static void
add_divisor_sum(idx_t idx,
                idx_t cur,
                const uint64_t *primes,
                const uint64_t *exps,
                size_t pcnt,
                idx_t n,
                seq_t *sum)
{
    if (idx == pcnt) {
        /* cur is a divisor of n */
        seq_t m = (seq_t)(n / cur);          /* n / d   */
        seq_t phi = m;                       /* φ(m) starts as m   */
        for (size_t j = 0; j < pcnt; ++j) {
            if (m % primes[j] == 0)          /* prime divides m   */
                phi -= phi / primes[j];
        }
        *sum += (seq_t)cur * phi;
        return;
    }

    uint64_t p   = primes[idx];
    uint64_t exp = exps[idx];
    idx_t   cur_pow = 1;
    for (uint64_t e = 0; e <= exp; ++e) {
        add_divisor_sum(idx + 1,
                        cur * cur_pow,
                        primes, exps,
                        pcnt, n, sum);
        cur_pow *= (idx_t)p;                  /* p^(e+1) */
    }
}

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
