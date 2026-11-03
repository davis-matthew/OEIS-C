/*
 * A364022  Expansion of Sum_{k>0} k * x^(4*k) / (1 + x^(5*k)).
 * https://oeis.org/A364022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 407, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9292 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A364022
 * Formula: a(n) = Sum_{d|n, n/d == 4 (mod 5)} (-1)^(n/d) * d
 */

#include <stdint.h>
#include <stddef.h>

static void accumulate_divs(size_t idx, idx_t cur_d,
                            const idx_t *primes, const idx_t *exps,
                            size_t cnt, idx_t n, seq_t *res)
{
    if (idx == cnt) {
        /* cur_d is a divisor of n */
        idx_t m = n / cur_d;
        if ((m % 5) == 4) {
            seq_t sign = (m & 1) ? -1 : 1;
            *res += sign * (seq_t)cur_d;
        }
        return;
    }
    idx_t p = primes[idx];
    idx_t e = exps[idx];
    idx_t d = cur_d;
    for (idx_t exp = 0; exp <= e; ++exp) {
        accumulate_divs(idx + 1, d, primes, exps, cnt, n, res);
        d *= p;                 /* increase exponent of current prime */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset starts at 1 */

    idx_t tmp = n;
    idx_t primes[64];
    idx_t exps[64];
    size_t cnt = 0;

    /* factorization: trial division */
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                ++e;
                tmp /= p;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (tmp > 1) {               /* remaining prime factor */
        primes[cnt] = tmp;
        exps[cnt]   = 1;
        ++cnt;
    }

    seq_t result = 0;
    accumulate_divs(0, 1, primes, exps, cnt, n, &result);
    return result;
}
