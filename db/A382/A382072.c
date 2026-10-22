/*
 * A382072  Lexicographically earliest sequence of distinct positive integers
 *          such that for any n > 0, n*a(n) is a factorial number.
 * https://oeis.org/A382072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest unused positive integer q such that there exists
 *            m ≥ 1 with m! = n·q
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 22, u128 n <= 31,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

/*--- Prime factor sieve ---------------------------------------------------*/
static idx_t spf_limit = 0;
static idx_t *spf = NULL;

static void ensure_spf(idx_t n)
{
    if (n <= spf_limit) return;
    if (spf) free(spf);
    build_spf(n, &spf);      /* spf[1] == 0, spf[i] > 0 for i>=2 */
    spf_limit = n;
}

/* Minimal m so that m! is divisible by n */
static idx_t min_m_for_n(idx_t n)
{
    if (n <= 1) return 1;
    ensure_spf(n);
    idx_t res = 1;
    idx_t tmp = n;
    while (tmp > 1) {
        idx_t p  = spf[tmp];
        idx_t e  = 0;
        while (tmp % p == 0) { tmp /= p; ++e; }

        /* Binary search for smallest m with exponent of p in m! ≥ e */
        idx_t lo = p, hi = p * e;     /* upper bound > minimal m */
        while (lo < hi) {
            idx_t mid = (lo + hi) >> 1;
            idx_t cnt  = 0, div = mid;
            while (div) { div /= p; cnt += div; }
            if (cnt >= e) hi = mid;
            else          lo = mid + 1;
        }
        if (lo > res) res = lo;
    }
    return res;
}

/*--- Factorial cache -----------------------------------------------------*/
static seq_t *fact_cache = NULL;
static size_t fact_cache_cap = 0;
static idx_t  fact_cache_len = 0;   /* number of stored factorials */

static void ensure_fact_cached(idx_t m)
{
    if (fact_cache_len > 0 && fact_cache_len-1 >= m) return;

    while (fact_cache_len <= m) {
        if (fact_cache_len == 0) { /* initialise 0! = 1 */
            fact_cache_cap = 4;
            fact_cache = (seq_t *)realloc(fact_cache, fact_cache_cap * sizeof(seq_t));
            fact_cache[0] = (seq_t)1;        /* 0! = 1 */
            fact_cache_len = 1;
        }
        if (fact_cache_len == fact_cache_cap) {
            fact_cache_cap *= 2;
            fact_cache = (seq_t *)realloc(fact_cache, fact_cache_cap * sizeof(seq_t));
        }
        idx_t next_m = fact_cache_len;         /* current m to compute: fact_cache_len-1 already stored */
        seq_t next_val = fact_cache[next_m-1] * (seq_t)next_m;
        fact_cache[next_m] = next_val;
        fact_cache_len++;
    }
}

/*--- Used value tracking -------------------------------------------------*/
static seq_t *used_vals = NULL;
static size_t used_len   = 0;
static size_t used_cap   = 0;

static bool used_contains(seq_t val)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used_vals[i] == val) return true;
    return false;
}

static void used_push(seq_t val)
{
    if (used_len == used_cap) {
        used_cap = used_cap ? used_cap * 2 : 4;
        used_vals = (seq_t *)realloc(used_vals, used_cap * sizeof(seq_t));
    }
    used_vals[used_len++] = val;
}

/*--- Cache for sequence terms --------------------------------------------*/
static seq_t *cache    = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;   /* number of terms stored, indices 1..cache_len */

/*--- Main function -------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for indices < offset */

    /* Compute terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        idx_t m = min_m_for_n(i);
        ensure_fact_cached(m);

        seq_t q = fact_cache[m] / (seq_t)i;
        while (used_contains(q)) {
            ++m;
            ensure_fact_cached(m);
            q = fact_cache[m] / (seq_t)i;
        }

        /* Store term */
        if (cache_len + 1 > cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 4;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = q;
        used_push(q);
    }

    return cache[n - 1];
}
