/*
 * A302007  Number of nX5 0..1 arrays with every element equal to 0, 1, 2 or 3
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A302007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) +82*a(n-2) +34*a(n-3) -90*a(n-4) -40*a(n-5)
 *            +37*a(n-6) Initial values: a(1)=16, a(2)=512, a(3)=13616,
 *            a(4)=369040, a(5)=9989376, a(6)=270422672
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc.h"

/* Static cache for memoized values */
static seq_t *cache      = NULL;  /* array of computed a(i), index i-1 stores a(i) */
static idx_t  cache_len  = 0;     /* number of initialized elements in cache   */
static idx_t  cache_cap  = 0;     /* allocated capacity of cache (in elements) */

/* Initial terms of the sequence (1‑based: index k maps to cache[k‑1]) */
static const seq_t init_vals[6] = { (seq_t)16,        (seq_t)512,
                                    (seq_t)13616,     (seq_t)369040,
                                    (seq_t)9989376,   (seq_t)270422672 };

/* Ensure that the cache has room for at least n+1 elements (indices 0..n) */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        /* First allocation: at least 10 001 to handle typical queries,
           but enlarge if n is larger. */
        cache_cap = (n > (idx_t)10000 ? n + 1 : (idx_t)10001);
        cache = seq_alloc(cache_cap);
        return;
    }
    if (cache_cap < n + 1) {
        /* Grow the cache by at least doubling its size. */
        idx_t new_cap = cache_cap * 2;
        if (n + 1 > new_cap)
            new_cap = n + 1;
        seq_t *new_cache = seq_alloc(new_cap);
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        /* Old memory is intentionally leaked; the process stays short‑lived. */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Compute the sequence up to and including term n (1‑based). */
static void compute_until(idx_t n)
{
    if (cache_len >= n)
        return;                     /* already computed */

    ensure_capacity(n);

    for (idx_t k = cache_len + 1; k <= n; ++k) {
        if (k <= 6) {
            cache[k - 1] = init_vals[k - 1];   /* copy initial terms */
        } else {
            /* Use the recurrence:
               a(k) = 24*a(k-1) + 82*a(k-2) + 34*a(k-3)
                     - 90*a(k-4) - 40*a(k-5) + 37*a(k-6)   */
            seq_t t1 = cache[k - 2] * (seq_t)24;
            seq_t t2 = cache[k - 3] * (seq_t)82;
            seq_t t3 = cache[k - 4] * (seq_t)34;
            seq_t t4 = cache[k - 5] * (seq_t)90;
            seq_t t5 = cache[k - 6] * (seq_t)40;
            seq_t t6 = cache[k - 7] * (seq_t)37;

            cache[k - 1] = t1 + t2 + t3 - t4 - t5 + t6;
        }
    }
    cache_len = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)                       /* undefined below the offset */
        return (seq_t)0;
    compute_until(n);
    return cache[n - 1];
}
