/*
 * A321438  a(n) = Sum_{d|n} (-1)^(n/d+1)*d^n.
 * https://oeis.org/A321438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d+1) * d^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

/* Recursively enumerate all divisors of a number given its prime factorisation.
   For each divisor d, add or subtract d^n to the running total. */
static void rec_divs(const uint64_t *primes, const int *exps, size_t total,
                     size_t idx, seq_t current, idx_t n, seq_t *acc)
{
    if (idx == total) {
        /* current is a divisor d */
        seq_t term = big_pow(current, (seq_t)n);
        /* sign depends on (n/d) parity: +1 if odd, -1 if even */
        if (((n / (idx_t)current) & 1U) == 1U)
            *acc += term;
        else
            *acc -= term;
        return;
    }
    uint64_t p = primes[idx];
    int e = exps[idx];
    seq_t val = current;
    for (int i = 0; i <= e; ++i) {
        rec_divs(primes, exps, total, idx + 1, val, n, acc);
        val *= (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not defined for n=0, but harmless */

    /* Factorise n using trial division (fast enough for n <= 10000). */
    uint64_t primes[16];
    int exps[16];
    size_t pcnt = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (m > 1) {
        primes[pcnt] = m;
        exps[pcnt] = 1;
        ++pcnt;
    }

    seq_t result = 0;
    rec_divs(primes, exps, pcnt, 0, (seq_t)1, n, &result);
    return result;
}
