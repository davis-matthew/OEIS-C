/*
 * A360026  a(n) = Sum_{k=0..floor(n/4)} (-1)^k * binomial(n-3*k,k) *
 *          Catalan(k).
 * https://oeis.org/A360026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0..⌊n/4⌋} (-1)^k * binomial(n-3k,k) * Catalan(k)
 *            Recurrence: for n ≥ 5 (n+4)*a(n) - 2(n+3)*a(n-1) + (n+2)*a(n-2)
 *            + 4(n-2)*a(n-4) + (-4n+10)*a(n-5) = 0 Base values:
 *            a(0)=a(1)=a(2)=a(3)=1, a(4)=0 The implementation uses a memoised
 *            recurrence. For 64‑bit integer builds (C mode) a 128‑bit...
 * Range:     i8 n <= 19, i16 n <= 30, i32 n <= 56, i64 n <= 105,
 *            i128 n <= 201, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed terms */
static idx_t cache_size = 0;  /* capacity of cache array */
static idx_t computed = 0;    /* number of terms computed (next free index) */

/* Grow the cache to hold at least 'need' elements. */
static void ensure_cache_size(idx_t need)
{
    if (need <= cache_size)
        return;

    size_t new_size = cache_size > 0 ? (size_t)cache_size : 4;
    while ((idx_t)new_size < need)
        new_size <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        abort();  /* unrecoverable infrastructure failure */

    /* Zero the newly allocated entries. */
    for (size_t i = cache_size; i < new_size; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_size = (idx_t)new_size;
}

/* Compute a(n) for all values up to n using the recurrence. */
static void compute_upto(idx_t n)
{
    if (n < computed)
        return;                       /* already computed up to at least n */

    ensure_cache_size(n + 1);

#ifdef __cplusplus
    /* C++ build – seq_t is an arbitrary‑precision type, use it directly */
    for (idx_t i = computed; i <= n; ++i) {
        if (i < 4) {
            cache[i] = 1;              /* a(0)=a(1)=a(2)=a(3)=1 */
        } else if (i == 4) {
            cache[i] = 0;              /* a(4)=0 */
        } else {
            /* (n+4)*a(n) - 2(n+3)*a(n-1) + (n+2)*a(n-2) + 4(n-2)*a(n-4)
               + (-4n+10)*a(n-5) = 0  =>  a(n) = [2(n+3)a(n-1) - (n+2)a(n-2)
               - 4(n-2)a(n-4) + (4n-10)a(n-5)]/(n+4) */
            seq_t termA = 2 * (seq_t)(i + 3) * cache[i - 1];
            seq_t termB = (seq_t)(i + 2) * cache[i - 2];
            seq_t termC = 4 * (seq_t)(i - 2) * cache[i - 4];
            seq_t termD = 2 * (seq_t)(2 * i - 5) * cache[i - 5];
            seq_t numerator = termA - termB - termC + termD;
            seq_t denominator = i + 4;
            cache[i] = numerator / denominator; /* exact division */
        }
    }
#else
    /* C build (seq_t is a fixed‑width integer) – use 128‑bit intermediates */
    for (idx_t i = computed; i <= n; ++i) {
        if (i < 4) {
            cache[i] = 1;
        } else if (i == 4) {
            cache[i] = 0;
        } else {
            /* use __int128 for the products to avoid overflow */
            __int128 termA = (__int128)2 * (i + 3) * cache[i - 1];
            __int128 termB = (__int128)(i + 2) * cache[i - 2];
            __int128 termC = (__int128)4 * (i - 2) * cache[i - 4];
            __int128 termD = (__int128)2 * (2 * (int64_t)i - 5) * cache[i - 5];
            __int128 numerator = termA - termB - termC + termD;
            seq_t result = (seq_t)(numerator / (i + 4));
            cache[i] = result;
        }
    }
#endif

    computed = n + 1;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    compute_upto(n);
    return cache[n];
}
