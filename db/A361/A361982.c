/*
 * A361982  a(n) = 1 + Sum_{k=2..n} (-1)^k * k * a(floor(n/k)).
 * https://oeis.org/A361982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = 1 + Sum_{k=2..n} (-1)^k * k * a(floor(n/k))
 * Range:     i8 n <= 25, i16 n <= 415, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive memoized implementation with O(n^2) time for computing all values up to n. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Forward declaration for recursive helper */
static seq_t compute_a(idx_t n);

/* Storage for memoization */
static seq_t *cache = NULL;      /* array of computed a(n) values */
static bool  *known = NULL;      /* flags indicating which values are known */
static size_t cache_size = 0;    /* current allocated size of the arrays */

/* Ensure the cache is large enough to hold values up to index n */
static void ensure_cache(idx_t n)
{
    if ((idx_t)(cache_size) > n) return;          /* already large enough */
    size_t old = cache_size;
    /* grow roughly double the size, but at least n+1 */
    size_t newsize = cache_size ? cache_size << 1 : 2;
    if (newsize <= (size_t)n + 1) newsize = (size_t)n + 1;
    /* Reallocate arrays */
    seq_t *newcache = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
    bool  *newknown = (bool*)realloc(known, newsize * sizeof(bool));
    if (!newcache || !newknown) {
        /* Allocation failed – abort; sequence building is impossible */
        abort();
    }
    cache = newcache;
    known = newknown;
    /* Initialize newly allocated flags to false */
    for (size_t i = old; i < newsize; ++i) known[i] = false;
    cache_size = newsize;
}

/* Recursive computation of a(n) with memoization */
static seq_t compute_a(idx_t n)
{
    ensure_cache(n);
    if (known[n]) return cache[n];
    if (n == (idx_t)1) {                     /* base case a(1) = 1 */
        cache[1] = (seq_t)1;
        known[1] = true;
        return cache[1];
    }
    seq_t sum = 0;
    for (idx_t k = 2; k <= n; ++k) {
        seq_t val  = compute_a(n / k);
        seq_t term = val * (seq_t)k;          /* k * a(floor(n/k)) */
        if ((k & 1ULL) == 0)                 /* (-1)^k positive when k even */
            sum += term;
        else
            sum -= term;
    }
    sum += (seq_t)1;                          /* the +1 in the recurrence */
    cache[n]  = sum;
    known[n]  = true;
    return sum;
}

/* Public API required by the build */
seq_t a(idx_t n)
{
    return compute_a(n);
}
