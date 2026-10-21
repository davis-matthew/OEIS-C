/*
 * A259615  a(0)=0, a(1)=a(2)=a(3)=a(4)=1; thereafter, a(n) = Sum_{k=1..5}
 *          a(n-k-(a(n-k) mod 5)).
 * https://oeis.org/A259615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=a(2)=a(3)=a(4)=1; for n>=5 a(n) = Σ_{k=1..5}
 *            a(n‑k‑(a(n‑k) mod 5))
 * Range:     u8 n <= 17, u16 n <= 30, u32 n <= 60, u64 n <= 98, u128 n <= 164,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cached values of the sequence */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of entries stored (highest index + 1) */
static size_t cache_cap = 0;   /* allocated capacity */

/* Ensure that cache can hold values up to index n */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;                 /* already available */

    size_t needed = n + 1;        /* indices 0..n inclusive */
    if (needed > cache_cap) {
        size_t new_cap = needed * 2;   /* growth factor */
        /* Portable cast of realloc return value */
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr)
            abort();            /* allocation failure */
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /* Compute missing values */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)0;
        } else if (i <= 4) {
            cache[i] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)0;
            for (int k = 1; k <= 5; ++k) {
                idx_t m = i - (idx_t)k;          /* n - k */
                seq_t ak = cache[m];
                idx_t shift = (idx_t)(ak % (seq_t)5); /* a(m) mod 5 */
                idx_t idx = m - shift;            /* n - k - (a(n-k) mod 5) */
                sum += cache[idx];
            }
            cache[i] = sum;
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
