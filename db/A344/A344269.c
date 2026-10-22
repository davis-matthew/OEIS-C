/*
 * A344269  Dirichlet g.f.: Product_{k>=2} (1 + k^(-s))^(6^(k-1)).
 * https://oeis.org/A344269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over all factorizations of n into distinct bases k≥2
 *            * k^e where 0≤e≤6^(k-1), of the product of binomial coefficients
 *            * C(6^(k-1), e). The recursion enumerates the bases in
 *            increasing order * to avoid double counting
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

#define MAX_SMALL_K 25   /* 6^(k-1) fits in uint64_t for k≤25 */

static uint64_t pow6_u64_cache[MAX_SMALL_K + 1] = {0}; /* 6^(k-1) for k≤25 */

/* Lazy evaluation of 6^(k-1) in uint64_t */
static uint64_t pow6_u64(idx_t k)
{
    if (k < 2) return 1ULL;
    if (pow6_u64_cache[k] == 0) {
        uint64_t v = 1;
        for (idx_t i = 1; i < k; ++i) v *= 6ULL;
        pow6_u64_cache[k] = v;
    }
    return pow6_u64_cache[k];
}

/* Cache for 6^(k-1) when seq_t offers arbitrary precision */
static seq_t *pow6_seq_cache = NULL;
static size_t pow6_seq_len = 0;

static seq_t pow6_seq_value(idx_t k)
{
    if (k < 2) return (seq_t)1;
    if ((size_t)k >= pow6_seq_len) {
        size_t new_len = k + 1;
        pow6_seq_cache = (seq_t *)realloc(pow6_seq_cache,
                                            new_len * sizeof(seq_t));
        for (size_t i = pow6_seq_len; i < new_len; ++i)
            pow6_seq_cache[i] = big_pow((seq_t)6, (seq_t)(i - 1));
        pow6_seq_len = new_len;
    }
    return pow6_seq_cache[k];
}

/* Compute C(6^(k-1), e).  If the value does not fit into seq_t,
 * return 0 and set *overflow to true. */
static seq_t binom_coeff(idx_t k, idx_t e, bool *overflow)
{
    *overflow = false;
    if (e == 0) return (seq_t)1;

    if (k <= MAX_SMALL_K) {
        uint64_t base = pow6_u64(k);
        __uint128_t r = 1;
        for (idx_t i = 1; i <= e; ++i) {
            r = r * (base - (i - 1));
            r = r / i;
            if (sizeof(seq_t) <= 8 && r > ( (__uint128_t)UINT64_MAX )) {
                *overflow = true;
                return 0;
            }
        }
        return (seq_t)r;
    }

    /* Arbitrary‑precision case */
    seq_t N = pow6_seq_value(k);
    seq_t r = 1;
    for (idx_t i = 1; i <= e; ++i) {
        seq_t term = N - (seq_t)(i - 1);
        r = seq_mul(r, term);
        r = r / (seq_t)i;
    }
    return r;
}

/* Memoisation for a(n) when start == 2 */
static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;

static seq_t _a_rec(idx_t n, idx_t start)
{
    if (n == 1) return (seq_t)1;

    if (start == 2 && n < a_cache_len && a_cache[n] != 0)
        return a_cache[n];

    seq_t sum = 0;

    for (idx_t k = start; k <= n; ++k) {
        if (n % k != 0) continue;

        /* Skip bases that would produce overflow for small seq_t */
        if (sizeof(seq_t) <= 8 && k > MAX_SMALL_K) continue;

        idx_t power = k;
        idx_t e = 1;
        while (power <= n && n % power == 0) {
            bool overflow;
            seq_t coeff = binom_coeff(k, e, &overflow);
            if (overflow) break;   /* further e will also overflow */

            seq_t sub = _a_rec(n / power, k + 1);
            seq_t prod = seq_mul(coeff, sub);
            sum += prod;

            if (power > n / k) break;  /* avoid overflow of power */
            power *= k;
            ++e;
        }
    }

    if (start == 2) {
        if (n >= a_cache_len) {
            size_t new_len = n + 1;
            a_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
            for (size_t i = a_cache_len; i < new_len; ++i) a_cache[i] = 0;
            a_cache_len = new_len;
        }
        a_cache[n] = sum;
    }

    return sum;
}

seq_t a(idx_t n)
{
    return _a_rec(n, 2);
}
