/*
 * A359547  Numbers such that they are not divisible by p^p for any prime p,
 *          but for some k-th arithmetic derivative (k >= 1) of n such a
 *          factor exists.
 * https://oeis.org/A359547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th natural number that itself is not divisible by
 *            any prime power p^p, but for some k ≥ 1 its k-th arithmetic
 *            derivative has this property. All calculations are carried out
 *            using 64‑bit arithmetic; the final value is returned as seq_t
 * Range:     u8 n <= 64, u16 n <= 8898, u32 n <= 8898, u64 n <= 8898,
 *            u128 n <= 8898, big any n
 * Helpers:   has_p_power_factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_p_power_factor_uint64.h"

/* Arithmetic derivative of a 64‑bit integer n.  The function assumes
   n ≥ 2.  It returns n' = Σ e_i * n / p_i over prime factorisation
   n = Π p_i^e_i. */
static uint64_t arithmetic_derivative_uint64(uint64_t n)
{
    uint64_t sum = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p = (p == 2 ? 3 : p + 2)) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* e * n / p fits into uint64_t for the range we need */
            sum += e * n / p;
        }
    }
    if (m > 1) /* remaining prime factor p with exponent 1 */
        sum += n / m;
    return sum;
}

/* Tests whether a number n (64‑bit) satisfies the defining
   property of the sequence. */
static bool good_candidate(uint64_t n)
{
    if (has_p_power_factor_uint64(n))
        return false;                /* n itself has p^p factor */

    uint64_t cur = n;
    while (cur > 1) {
        cur = arithmetic_derivative_uint64(cur);
        if (cur == 0 || cur == 1)
            break;
        if (has_p_power_factor_uint64(cur))
            return true;
    }
    return false;
}

/* Main sequence function.  It memoises the sequence values in a static
   array so that calls in any order are efficient.  The array grows
   as needed. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* 0‑based: cache[0] = a(1) */
    static idx_t cache_size = 0;
    static uint64_t last_checked = 1;   /* last integer examined */

    /* Expand the cache until it contains at least n terms. */
    while (cache_size < n) {
        uint64_t m = ++last_checked;
        if (good_candidate(m)) {
            /* Ensure capacity. */
            if (cache_size == 0) {
                cache = (seq_t *)malloc(8 * sizeof(seq_t));
                if (cache == NULL) exit(1);
            } else if (cache_size % 8 == 0) {
                seq_t *tmp = (seq_t *)realloc(cache,
                             (cache_size + 8) * sizeof(seq_t));
                if (tmp == NULL) exit(1);
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)m;
        }
    }

    return cache[n - 1];
}
