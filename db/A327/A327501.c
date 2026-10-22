/*
 * A327501  Maximum divisor of n that is 1 or not a perfect power.
 * https://oeis.org/A327501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum divisor d of n such that d == 1 or d is not a
 *            perfect power
 * Range:     u8 n <= 64, u16 n <= 1976, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_perfect_power_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n, generates all its divisors,
   tests each divisor for being a perfect power, and returns
   the largest divisor that satisfies the condition. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_perfect_power_seq.h"

/* Recursively generate all divisors from the prime factorization
   and keep track of the maximum divisor that is either 1 or not a
   perfect power. */
static void generate_divisors(const uint64_t *primes,
                              const int *exps,
                              size_t cnt,
                              size_t idx,
                              seq_t current,
                              seq_t *max_div)
{
    if (idx == cnt) {
        if (current == (seq_t)1) {
            if (current > *max_div) *max_div = current;
        } else if (!is_perfect_power_seq(current)) {
            if (current > *max_div) *max_div = current;
        }
        return;
    }

    seq_t prod = current;
    for (int e = 0; e <= exps[idx]; ++e) {
        generate_divisors(primes, exps, cnt, idx + 1, prod, max_div);
        prod *= (seq_t)primes[idx];
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;   /* n=1: only divisor is 1 */

    /* Factor n into primes.  Since idx_t is uint64_t, n fits in uint64_t. */
    uint64_t primes[64];
    int      exps  [64];
    size_t   cnt   = 0;

    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (m > 1) {
        primes[cnt] = m;
        exps[cnt]   = 1;
        ++cnt;
    }

    seq_t max_div = (seq_t)1;  /* divisor 1 always qualifies */
    generate_divisors(primes, exps, cnt, 0, (seq_t)1, &max_div);
    return max_div;
}
