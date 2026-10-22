/*
 * A355230  E.g.f. A(x) satisfies A'(x) = 1 - log(1-x) * A(2*x).
 * https://oeis.org/A355230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A355230
   a(0) = 0, a(1) = 1
   a(n+1) = Sum_{k=1..n-1} 2^(n-k) * (k-1)! * binomial(n,k) * a(n-k)  (n ≥ 1)
   (recurrence over n with offset 0) */

#include <stdlib.h>

/* The build supplies the types seq_t (value type) and idx_t (index type). */

/* Forward declarations of helper functions that allocate variable‐size arrays. */
static void *alloc_seq_array(size_t n);

static seq_t *cache      = NULL;   /* a[0..len-1] */
static idx_t  cache_len = 0;
static size_t cache_cap = 0;

static seq_t *fact       = NULL;   /* fact[i] = i!   */
static idx_t  fact_len  = 0;
static size_t fact_cap  = 0;

static seq_t *pow2       = NULL;   /* pow2[i] = 2^i   */
static idx_t  pow2_len  = 0;
static size_t pow2_cap  = 0;

/* Allocate an array of seq_t of the given length.  */
static void *alloc_seq_array(size_t n)
{
    return malloc(sizeof(seq_t) * n);
}

/* Ensure that fact[0..n] are available. */
static void ensure_fact(idx_t n)
{
    if (n < fact_len)
        return;

    size_t new_cap = (n + 1 > fact_cap) ? (n + 1) * 2 : fact_cap;
    if (new_cap < n + 1)
        new_cap = n + 1;
    if (new_cap < 16)
        new_cap = 16;

    if (fact == NULL) {
        fact = (seq_t *)alloc_seq_array(new_cap);
    } else {
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        fact = tmp;
    }
    fact_cap = new_cap;

    for (idx_t i = fact_len; i <= n; ++i) {
        if (i == 0)
            fact[i] = (seq_t)1;
        else
            fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_len = n + 1;
}

/* Ensure that pow2[0..n] are available. */
static void ensure_pow2(idx_t n)
{
    if (n < pow2_len)
        return;

    size_t new_cap = (n + 1 > pow2_cap) ? (n + 1) * 2 : pow2_cap;
    if (new_cap < n + 1)
        new_cap = n + 1;
    if (new_cap < 16)
        new_cap = 16;

    if (pow2 == NULL) {
        pow2 = (seq_t *)alloc_seq_array(new_cap);
    } else {
        seq_t *tmp = (seq_t *)realloc(pow2, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        pow2 = tmp;
    }
    pow2_cap = new_cap;

    for (idx_t i = pow2_len; i <= n; ++i) {
        if (i == 0)
            pow2[i] = (seq_t)1;
        else
            pow2[i] = pow2[i - 1] * (seq_t)2;
    }
    pow2_len = n + 1;
}

/* Ensure cache[0..n] are computed. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    size_t new_cap = (n + 1 > cache_cap) ? (n + 1) * 2 : cache_cap;
    if (new_cap < n + 1)
        new_cap = n + 1;
    if (new_cap < 16)
        new_cap = 16;

    if (cache == NULL) {
        cache = (seq_t *)alloc_seq_array(new_cap);
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }
    cache_cap = new_cap;

    /* Base values */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache_len = 1;
    }
    if (cache_len == 1 && n >= 1) {
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Pre‑compute factorials and powers of two up to needed sizes. */
    ensure_fact(n);          /* ensures at least fact[n] (k-1)! up to n-1 */
    ensure_pow2(n);          /* ensures at least pow2[n] */

    /* Compute new terms */
    for (idx_t cur = cache_len; cur <= n; ++cur) {
        if (cur == 0) {
            cache[0] = (seq_t)0;
            continue;
        }
        if (cur == 1) {
            cache[1] = (seq_t)1;
            continue;
        }

        idx_t N = cur - 1;          /* n in the recurrence */
        seq_t sum = (seq_t)0;

        seq_t binom = (seq_t)1;
        for (idx_t k = 1; k <= N - 1; ++k) {
            binom = binom * (seq_t)(N - k + 1) / (seq_t)k; /* C(N,k) */
            idx_t exp = N - k;                            /* exponent for 2 */
            seq_t term = pow2[exp];
            term = term * fact[k - 1];                    /* * (k-1)! */
            term = term * binom;                          /* * binomial(N,k) */
            term = term * cache[cur - 1 - k];             /* * a(N-k) */
            sum += term;
        }
        cache[cur] = sum;
    }

    cache_len = n + 1;
}

/* Public entry point required by the build system. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
