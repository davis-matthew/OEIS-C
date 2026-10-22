/*
 * A364235  Expansion of Sum_{k>=0} x^(3*k+2) / (1 + x^(3*k+2))^2.
 * https://oeis.org/A364235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 159, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A364235, Expansion of Sum_{k>=0} x^(3*k+2) / (1 + x^(3*k+2))^2
   Formula: a(n) = Sum_{d|n, n/d==2 (mod 3)} (-1)^(d+1) * d   (n>=1)  */
/* Note: a(n) = sum_{d|n, (n/d) % 3 == 2} { (+d if d odd) (-d if d even) } */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes.h"

static idx_t *primes = NULL;      /* array of primes up to current limit */
static size_t primes_cnt = 0;      /* number of primes in array */
static idx_t primes_limit = 0;    /* current maximum prime value (sieve limit) */

/* integer square root: floor(sqrt(x)) for unsigned 64‑bit integers */
static inline idx_t isqrt_u64(idx_t x)
{
    if (x < 2) return x;
    idx_t r = 0;
    idx_t bit = 1ULL << 62;          /* highest power of 4 <= 2^64 */
    while (bit) {
        idx_t candidate = r | bit;
        if (candidate <= x / candidate) r = candidate;
        bit >>= 1;
    }
    return r;
}

/* Recursively enumerate all divisors and accumulate the sum */
static void dfs_div(size_t idx,
                    seq_t cur,
                    const idx_t *pr,
                    const idx_t *exp,
                    size_t fac_cnt,
                    idx_t n,
                    seq_t *res)
{
    if (idx == fac_cnt) {
        seq_t d = cur;
        seq_t q = n / d;
        if ((q % 3) == 2) {
            if ((d & 1) != 0) *res += d;   /* odd divisor -> +d */
            else              *res -= d;   /* even divisor -> -d */
        }
        return;
    }
    seq_t val = cur;
    for (size_t i = 0; i <= exp[idx]; ++i) {
        dfs_div(idx + 1, val, pr, exp, fac_cnt, n, res);
        if (i < exp[idx]) val = val * (seq_t)pr[idx];
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* never requested, but safe */

    /* Ensure we have a list of primes up to sqrt(n) */
    idx_t sqrt_n = isqrt_u64(n);
    if (sqrt_n > primes_limit) {
        free(primes);
        build_primes(sqrt_n, &primes, &primes_cnt);
        primes_limit = sqrt_n;
    }

    /* Factor n into primes and exponents */
    idx_t fac_p[64];
    idx_t fac_e[64];
    size_t fac_cnt = 0;
    idx_t temp = n;
    for (size_t i = 0; i < primes_cnt && (primes[i] * primes[i] <= temp); ++i) {
        if (temp % primes[i] == 0) {
            idx_t p = primes[i];
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            fac_p[fac_cnt] = p;
            fac_e[fac_cnt] = e;
            ++fac_cnt;
        }
    }
    if (temp > 1) {        /* remaining prime factor */
        fac_p[fac_cnt] = temp;
        fac_e[fac_cnt] = 1;
        ++fac_cnt;
    }

    /* Enumerate divisors and compute the sum */
    seq_t result = 0;
    dfs_div(0, (seq_t)1, fac_p, fac_e, fac_cnt, n, &result);
    return result;
}
