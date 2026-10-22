/*
 * A382678  a(n) = Sum_{k=0..n} (k!)^2 * binomial(k+3,3) *
 *          Stirling2(n+1,k+1)^2.
 * https://oeis.org/A382678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k!)^2 * binomial(k+3,3) *
 *            Stirling2(n+1,k+1)^2 (OEIS A382678) This implementation keeps
 *            factorials, Stirling numbers of the second kind and already
 *            computed a(n) values in static caches. It works for built‑in
 *            unsigned integer types, for unsigned __int128 and for
 *            arbitrary‑pre...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

/* ---------- static cache state ------------------------------------- */
#ifdef __cplusplus
/* C++ : use std::vector for proper construction/destruction */
static std::vector<seq_t> cache;
static std::vector<seq_t> fact;
static std::vector<seq_t> s_prev;
static std::vector<seq_t> s_curr;
static idx_t max_n_computed = 0;
static idx_t max_m_computed = 0;
#else
/* C : use raw pointers (seq_t must be trivially constructible)      */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static seq_t *fact = NULL;
static size_t fact_size = 0;
static seq_t *s_prev = NULL;
static seq_t *s_curr = NULL;
static size_t s_size = 0;
static idx_t max_n_computed = 0;
static idx_t max_m_computed = 0;
#endif

/* ---------- helper functions --------------------------------------- */
#ifdef __cplusplus
/* Ensure factorials 0..n exist in vector `fact`. */
static void ensure_factorials(idx_t n)
{
    if (fact.size() <= (size_t)n) {
        size_t old = fact.size();
        fact.resize((size_t)n + 1);
        for (size_t i = old; i <= (size_t)n; ++i) {
            if (i == 0) fact[i] = 1;
            else fact[i] = fact[i - 1] * (seq_t)i;
        }
    }
}

/* Ensure Stirling row S(m,*) for m = n+1.  `s_prev` becomes that row. */
static void ensure_stirling_row(idx_t n)
{
    idx_t desired_m = n + 1;
    if (desired_m <= max_m_computed) return;

    size_t new_len = (size_t)desired_m + 1;
    if (s_prev.size() < new_len) {
        s_prev.resize(new_len);
        s_curr.resize(new_len);
    }

    /* initialise row 0 if we have not done anything before */
    if (max_m_computed == 0 && s_prev[0] != (seq_t)1)
        s_prev[0] = 1;

    for (idx_t m = max_m_computed + 1; m <= desired_m; ++m) {
        size_t mi = (size_t)m;
        s_curr[0] = 0;
        for (size_t r = 1; r <= mi; ++r) {
            seq_t part1 = (seq_t)r * s_prev[r];
            seq_t part2 = s_prev[r - 1];
            s_curr[r] = part1 + part2;
        }
        std::swap(s_prev, s_curr);
    }
    max_m_computed = desired_m;
}

/* Store a value in the cache.  The cache vector grows automatically. */
static void store_in_cache(idx_t n, seq_t value)
{
    size_t idx = (size_t)n;
    if (cache.size() <= idx) cache.resize(idx + 1);
    cache[idx] = value;
    if (idx + 1 > max_n_computed) max_n_computed = idx + 1;
}
#else
/* Ensure factorials 0..n exist in raw array `fact`. */
static void ensure_factorials(idx_t n)
{
    if (fact_size <= n) {
        size_t old = fact_size;
        fact = (seq_t *)realloc(fact, ((size_t)n + 1) * sizeof(seq_t));
        if (!fact) exit(EXIT_FAILURE);
        for (size_t i = old; i <= (size_t)n; ++i) {
            if (i == 0) fact[i] = 1;
            else fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_size = n + 1;
    }
}

/* Ensure Stirling row S(m,*) for m = n+1.  `s_prev` becomes that row. */
static void ensure_stirling_row(idx_t n)
{
    idx_t desired_m = n + 1;
    if (desired_m <= max_m_computed) return;

    size_t new_len = (size_t)desired_m + 1;
    if (s_prev == NULL) {
        s_prev = (seq_t *)calloc(new_len, sizeof(seq_t));
        s_curr = (seq_t *)calloc(new_len, sizeof(seq_t));
        s_size = new_len;
    } else if (s_size < new_len) {
        s_prev = (seq_t *)realloc(s_prev, new_len * sizeof(seq_t));
        s_curr = (seq_t *)realloc(s_curr, new_len * sizeof(seq_t));
        if (!s_prev || !s_curr) exit(EXIT_FAILURE);
        for (size_t i = s_size; i < new_len; ++i) {
            s_prev[i] = 0;
            s_curr[i] = 0;
        }
        s_size = new_len;
    }

    if (max_m_computed == 0 && s_prev[0] != (seq_t)1)
        s_prev[0] = 1;

    for (idx_t m = max_m_computed + 1; m <= desired_m; ++m) {
        size_t mi = (size_t)m;
        s_curr[0] = 0;
        for (size_t r = 1; r <= mi; ++r) {
            seq_t part1 = (seq_t)r * s_prev[r];
            seq_t part2 = s_prev[r - 1];
            s_curr[r] = part1 + part2;
        }
        seq_t *tmp = s_prev;
        s_prev = s_curr;
        s_curr = tmp;
    }
    max_m_computed = desired_m;
}

/* Store a value in the cache. */
static void store_in_cache(idx_t n, seq_t value)
{
    size_t idx = (size_t)n;
    if (cache == NULL) {
        cache = (seq_t *)calloc(idx + 1, sizeof(seq_t));
        cache_size = idx + 1;
    } else if (cache_size <= idx) {
        cache = (seq_t *)realloc(cache, (idx + 1) * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (size_t i = cache_size; i <= idx; ++i) cache[i] = 0;
        cache_size = idx + 1;
    }
    cache[idx] = value;
    if (idx + 1 > max_n_computed) max_n_computed = idx + 1;
}
#endif

/* ---------- public sequence function -------------------------------- */
seq_t a(idx_t n)
{
    /* Return cached value if we already computed it. */
    if (n < max_n_computed) {
#ifdef __cplusplus
        return cache[(size_t)n];
#else
        return cache[(size_t)n];
#endif
    }

    /* Prepare needed data. */
    ensure_factorials(n);
    ensure_stirling_row(n);

    /* Compute the sum. */
    seq_t sum = 0;
    for (size_t k = 0; k <= (size_t)n; ++k) {
        seq_t fk    = fact[(size_t)k];     /* k!                     */
        seq_t bin   = (seq_t)((k + 3) * (k + 2) * (k + 1)) / 6;  /* C(k+3,3) */
        seq_t Bk    = fk * fk * bin;      /* (k!)^2 * C(k+3,3)      */
        seq_t sval  = s_prev[(size_t)k + 1]; /* S(n+1, k+1)            */
        seq_t term  = Bk * (sval * sval);
        sum += term;
    }

    /* Cache and return. */
    store_in_cache(n, sum);
    return sum;
}
