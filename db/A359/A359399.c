/*
 * A359399  a(1) = 1; a(n) = Sum_{k=2..n} k * a(floor(n/k)).
 * https://oeis.org/A359399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n=1, otherwise a(n) = Sum_{k=2..n} k *
 *            a(floor(n/k))
 * Range:     u8 n <= 14, u16 n <= 110, u32 n <= 6479, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
/* In C++ we must use 'new'/'delete' so objects are constructed. */
static seq_t *cache = nullptr;
static idx_t   cache_cap = 0;
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;
    size_t new_cap = n + 1;
    seq_t *new_cache = new seq_t[new_cap];          /* default‑construct all */
    /* copy old values */
    for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
    /* delete old array */
    delete[] cache;
    cache = new_cache;
    cache_cap = new_cap;
}
#else
/* In C we can use malloc/realloc. */
static seq_t *cache = NULL;
static idx_t   cache_cap = 0;
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;
    size_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    /* initialize the newly allocated cells to 0 */
    memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n=0; guard */
    ensure_cache(n);
    /* base case */
    if (cache[1] == 0) cache[1] = (seq_t)1;

    if (cache[n] != 0) return cache[n];

    /* compute sequentially up to n, each term using the recurrence */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != 0) continue;      /* already known (may happen via a previous call) */

        seq_t sum = 0;
        idx_t k = 2;
        while (k <= i) {
            idx_t q = i / k;              /* floor(i/k) */
            idx_t r = i / q;              /* largest k with the same quotient */
            if (r > i) r = i;             /* safety, though r should never exceed i */

            /* sum of the arithmetic progression k..r */
            seq_t kk   = (seq_t)k;
            seq_t rr   = (seq_t)r;
            seq_t cnt  = (seq_t)(r - k + 1);
            seq_t mid  = (kk + rr) * cnt / 2;   /* (sum of k..r) */

            sum += mid * cache[q];
            k = r + 1;
        }
        cache[i] = sum;
    }
    return cache[n];
}
