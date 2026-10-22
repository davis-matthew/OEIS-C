/*
 * A354734  a(0) = a(1) = 1; a(n) = 3 * Sum_{k=0..n-2} a(k) * a(n-k-2).
 * https://oeis.org/A354734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1; a(n) = 3 * Σ_{k=0}^{n-2} a(k) * a(n-k-2)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t, NULL */

/* Use the supplied integer types.  The build defines seq_t and idx_t. */
#if defined(__cplusplus)
/* C++   – allocate objects with new[] so that constructors run. */
#include <cstring>   /* memcpy */
static seq_t *cache = nullptr;
static size_t cap   = 0;
static idx_t  last  = (idx_t)-1;   /* sentinel – nothing computed yet */

static void ensure_cache(idx_t n)
{
    if (!cache) {                               /* first allocation */
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 16) new_cap = 16;
        cache = new seq_t[new_cap];
        cap   = new_cap;
        last  = (idx_t)-1;
    } else if ((size_t)n + 1 > cap) {           /* grow cache */
        size_t new_cap = cap << 1;
        if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *new_cache = new seq_t[new_cap];
        for (size_t i = 0; i < cap; ++i) new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
        cap   = new_cap;
    }
}
#else
/* C     – use malloc / free; seq_t is assumed to be a plain‑old‑data type. */
#include <stdlib.h>   /* malloc, free, memcpy */
static seq_t *cache = NULL;
static size_t cap   = 0;
static idx_t  last  = (idx_t)-1;

static void ensure_cache(idx_t n)
{
    if (!cache) {                               /* first allocation */
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 16) new_cap = 16;
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        cap   = new_cap;
        last  = (idx_t)-1;
    } else if ((size_t)n + 1 > cap) {           /* grow cache */
        size_t new_cap = cap << 1;
        if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        memcpy(new_cache, cache, cap * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cap   = new_cap;
    }
}
#endif

/* Compute the nth term of A354734.  The function is thread‑safe with
   respect to its own state; concurrent calls may race on the static
   cache, which is acceptable for the evaluation environment. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Initialise the first two terms once. */
    if (last == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        last = 1;
    }

    if (n <= last) return cache[n];

    /* Compute all missing values up to n. */
    for (idx_t cur = last + 1; cur <= n; ++cur) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= cur - 2; ++k) {
            sum += cache[k] * cache[cur - k - 2];
        }
        cache[cur] = (seq_t)3 * sum;
    }
    last = n;
    return cache[n];
}
