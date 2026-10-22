/*
 * A390856  a(n) = Sum_{k=0..n} (-1)^k * binomial(n+2*k+1,n-k) *
 *          Fibonacci(k+1).
 * https://oeis.org/A390856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * binomial(n+2*k+1,n-k) *
 *            Fibonacci(k+1) Recurrence: a(n) = 5*a(n-1) - 11*a(n-2) +
 *            17*a(n-3) - 14*a(n-4) + 6*a(n-5) - a(n-6) Initial values:
 *            a(0)=1, a(1)=1, a(2)=0, a(3)=2, a(4)=14, a(5)=40
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic cache of computed terms */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_len = 0;  /* number of terms already computed */

/* Ensure the cache can hold at least (n+1) terms. Handles both C and C++. */
static void ensure_cache_cap(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;

#ifdef __cplusplus
    /* C++ uses operator new[] for non‑trivial types. */
    seq_t *tmp = new (std::nothrow) seq_t[new_cap];
    if (!tmp) abort();
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
        delete [] cache;
    }
    cache = tmp;
#else
    /* C uses malloc/realloc and plain casting. */
    void *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = (seq_t *)tmp;
#endif
    cache_cap = new_cap;
}

/* Initialise the first six terms of the sequence. */
static void init_cache(void)
{
    if (cache_len != 0) return;          /* already initialised */

    ensure_cache_cap(5);                 /* room for indices 0..5 */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)0;
    cache[3] = (seq_t)2;
    cache[4] = (seq_t)14;
    cache[5] = (seq_t)40;
    cache_len = 6;
}

/* Compute the n‑th term of A390856. */
seq_t a(idx_t n)
{
    init_cache();
    ensure_cache_cap(n);

    if ((size_t)n < cache_len) return cache[(size_t)n];

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val =
            5  * cache[(size_t)(i - 1)]
          - 11 * cache[(size_t)(i - 2)]
          + 17 * cache[(size_t)(i - 3)]
          - 14 * cache[(size_t)(i - 4)]
          +  6 * cache[(size_t)(i - 5)]
          -   cache[(size_t)(i - 6)];
        cache[(size_t)i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
