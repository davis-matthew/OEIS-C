/*
 * A297221  Number of n X 5 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 1 neighboring 1s.
 * https://oeis.org/A297221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A297221
   Empirical recurrence:
     a(n) = a(n-1) + 16·a(n-2) + 45·a(n-3) + 39·a(n-4)
            - 3·a(n-5) - 21·a(n-6) + 8·a(n-7) - 4·a(n-8) + a(n-9)
   Initial values (n = 1 … 9):
     a(1)=6, a(2)=33, a(3)=182, a(4)=997, a(5)=5570,
     a(6)=30946, a(7)=171851, a(8)=955316, a(9)=5308160
   The function below memoises and returns a(n) for any n≥1.
*/

#include <stdlib.h>

/* memoisation cache */
static seq_t *cache = NULL;
static idx_t cached_size = 0;

/* hard‑coded first nine terms (needed to seed the recurrence) */
static const seq_t init_vals[9] = {
    (seq_t)6ULL,
    (seq_t)33ULL,
    (seq_t)182ULL,
    (seq_t)997ULL,
    (seq_t)5570ULL,
    (seq_t)30946ULL,
    (seq_t)171851ULL,
    (seq_t)955316ULL,
    (seq_t)5308160ULL
};

/* grow the cache so that it contains values up to index n (1‑based) */
static void ensure_cache(idx_t n)
{
    if (n <= cached_size)
        return;

    /* reallocate to hold n terms */
    size_t new_len = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) abort();          /* allocation failed */
    cache = new_cache;

    /* initialise any missing entries */
    for (idx_t k = cached_size + 1; k <= n; ++k) {
        if (k <= 9) {
            cache[k - 1] = init_vals[k - 1];
        } else {
            idx_t idx = k - 1;               /* zero‑based index of a(k) */
            seq_t v  = cache[idx - 1];       /* a(k-1)          */
            v += 16 * cache[idx - 2];        /* a(k-2)          */
            v += 45 * cache[idx - 3];        /* a(k-3)          */
            v += 39 * cache[idx - 4];        /* a(k-4)          */
            v -=  3 * cache[idx - 5];        /* a(k-5)          */
            v -= 21 * cache[idx - 6];        /* a(k-6)          */
            v +=  8 * cache[idx - 7];        /* a(k-7)          */
            v -=  4 * cache[idx - 8];        /* a(k-8)          */
            v +=   cache[idx - 9];           /* a(k-9)          */
            cache[idx] = v;
        }
    }
    cached_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* defined only for n≥1 */
    ensure_cache(n);
    return cache[n - 1];
}
