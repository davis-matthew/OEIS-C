/*
 * A246466  Catalan number analogs for A246465, the generalized binomial
 *          coefficients for A003557.
 * https://oeis.org/A246466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A246465(2n,n) / A003557(n+1) * where A246465(m,k) =
 *            (∏_{i=1}^m s(i)) / (∏_{i=1}^k s(i) ∏_{i=1}^{m-k} s(i)), * s(i) =
 *            i / rad(i) (A003557)
 * Range:     u8 n <= 13, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/gcd.h"

static seq_t *a_cache      = NULL;   /* a_cache[i] = a(i) */
static idx_t  a_cache_len  = 0;     /* number of stored a-values */
static uint32_t *spf;                /* smallest prime factor array for range */
static idx_t spf_len = 0;           /* size of spf array (indexes 0..spf_len-1) */
static idx_t *s_arr;                /* s(i) values, i from 0..s_arr_len-1 */
static idx_t s_arr_len = 0;

/* Build spf and s_arr up to max_n (inclusive). */
static void build_spf_and_s(idx_t max_n)
{
    /* size_t is guaranteed large enough for array sizes in this context. */
    size_t n = (size_t)max_n + 1;
    if ((size_t)spf_len >= n && (size_t)s_arr_len >= n)
        return; /* already built */

    /* Reallocate spf array. */
    spf = (uint32_t *)realloc(spf, n * sizeof(uint32_t));
    if (!spf) abort();
    spf_len = (idx_t)n;

    /* Sieve of Eratosthenes to fill spf. */
    for (size_t i = 0; i < n; ++i) spf[i] = 0;
    spf[0] = 0;
    spf[1] = 1;
    for (size_t i = 2; i < n; ++i) {
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            if (i * i < n) {
                for (size_t j = i * i; j < n; j += i) {
                    if (!spf[j]) spf[j] = (uint32_t)i;
                }
            }
        }
    }

    /* Reallocate s_arr array. */
    s_arr = (idx_t *)realloc(s_arr, n * sizeof(idx_t));
    if (!s_arr) abort();
    s_arr_len = max_n + 1;

    /* rad array temporarily used during construction. */
    idx_t *rad = (idx_t *)calloc(n, sizeof(idx_t));
    rad[1] = 1;
    s_arr[0] = 0;   /* not used */
    s_arr[1] = 1;   /* s(1) = 1 */

    for (idx_t i = 2; i <= max_n; ++i) {
        uint32_t p = spf[i];
        idx_t j = i / p;
        if (spf[j] == p) rad[i] = rad[j];
        else             rad[i] = rad[j] * (idx_t)p;
        s_arr[i] = i / rad[i];
    }

    free(rad);
}

/* Expand a_cache up to value n (inclusive). */
static void expand_a_cache(idx_t n)
{
    if (n < a_cache_len) return; /* already computed */

    /* Ensure s_arr computed up to 2n. */
    idx_t need_limit = 2 * n;
    build_spf_and_s(need_limit);

    /* Reallocate a_cache if needed. */
    size_t new_len = (size_t)n + 1;
    a_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
    if (!a_cache) abort();
    if (a_cache_len == 0) a_cache[0] = (seq_t)1; /* a(0)=1 */

    /* Compute new values iteratively. */
    for (idx_t i = a_cache_len; i <= n; ++i) {
        if (i == 0) continue; /* already set */
        idx_t cur = i - 1;    /* current index for previous value */
        /* S values needed: s(n+1), s(n+2), s(2n+1), s(2n+2) */
        idx_t s_n1   = s_arr[cur + 1];
        idx_t s_n2   = s_arr[cur + 2];
        idx_t s_2n1  = s_arr[2 * cur + 1];
        idx_t s_2n2  = s_arr[2 * cur + 2];

        seq_t num = (seq_t)s_2n1 * (seq_t)s_2n2;   /* numerator product */
        seq_t den = (seq_t)s_n1 * (seq_t)s_n2;     /* denominator product */

        seq_t a_cur = a_cache[cur];

        /* Reduce a_cur with den. */
        seq_t g1 = gcd(a_cur, den);
        a_cur /= g1;
        den   /= g1;

        /* Reduce num with remaining den. */
        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;

        /* After reductions, den should divide a_cur * num exactly. */
        seq_t a_new = a_cur * num;
        if (den > 1) a_new /= den;

        a_cache[i] = a_new;
    }

    a_cache_len = new_len;
}

seq_t a(idx_t n) {
    expand_a_cache(n);
    return a_cache[n];
}
