/*
 * A265987  Number of n X 3 integer arrays with each element equal to the
 *          number of horizontal and antidiagonal neighbors not equal to
 *          itself.
 * https://oeis.org/A265987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2*a(n-1) + 3*a(n-2) - 4*a(n-3)   (n ≥ 4)
 * Initial values: a(1)=2, a(2)=2, a(3)=6
 * This implementation memoizes the sequence so that each call works
 * in O(n) time overall, regardless of argument order.  It compiles
 * in C‐11 and C++17.  For C it uses malloc/realloc; for C++ it uses
 * std::vector to ensure proper construction of complex seq_t types.
 */
#include <stddef.h>  /* for size_t */
#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>  /* for malloc, realloc, abort */
#endif

/* ---------- static storage ---------- */
#ifdef __cplusplus
static std::vector<seq_t> cache;          /* 1‑based storage, cache[0] unused */
static idx_t cache_filled = 0;            /* largest index with a value set   */
#else
static seq_t *cache = NULL;              /* 1‑based storage, cache[0] unused */
static size_t cache_alloc = 0;            /* number of slots allocated        */
static idx_t cache_filled = 0;            /* largest index with a value set   */
#endif

/* ---------- capacity helpers ---------- */
#ifdef __cplusplus
static void ensure_capacity(idx_t n)
{
    if (cache.size() <= (size_t)n)               /* need indices 0..n */
        cache.resize(n + 1);
}
#else
static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) > cache_alloc) {          /* need indices 0..n */
        seq_t *tmp = (seq_t*)realloc(cache,
                                     (n + 1) * sizeof(seq_t));
        if (!tmp) abort();                       /* OOM – terminate */
        cache = tmp;
        cache_alloc = (size_t)(n + 1);
    }
}
#endif

/* ---------- initialise first three terms ---------- */
static void initialise_bases(void)
{
    if (cache_filled >= 3) return;               /* already ready */

    /* Ensure we can write to indices 1..3 */
    ensure_capacity(3);

#ifdef __cplusplus
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)6;
#else
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)6;
#endif
    cache_filled = 3;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                        /* offset is 1 */

    ensure_capacity(n);
    initialise_bases();

    for (idx_t k = cache_filled + 1; k <= n; ++k) {
        seq_t val = 2 * cache[k - 1] + 3 * cache[k - 2]
                    - 4 * cache[k - 3];
        cache[k] = val;
    }
    cache_filled = n;

    return cache[n];
}
