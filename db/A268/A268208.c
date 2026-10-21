/*
 * A268208  Number of paths from (0,0) to (n,n) using only steps North,
 *          Northeast and East (i.e., steps E(1,0), D(1,1), and N(0,1)) that
 *          do not cross y=x "vertically".
 * https://oeis.org/A268208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A268208
 *  a(n) = number of paths from (0,0) to (n,n) using steps
 *          (1,0), (1,1) and (0,1) that do not cross y=x vertically.
 *  Recurrence (holds for n ≥ 4):
 *      a(n) = [(7n+2)a(n‑1) – (7n‑16)a(n‑2) + (n‑4)a(n‑3)] / (n+2)
 *  Initial values: a(0)=1, a(1)=3, a(2)=12, a(3)=52.
 *
 *  The function below caches already computed terms so that
 *  subsequent calls are O(1).  The recurrence never needs to
 *  handle negative intermediate values because n‑4 ≥ 0 for n ≥ 4.
 */

#include <stdlib.h>

/* Static cache for computed terms.  cache[i] holds a(i).  */
static seq_t *cache = NULL;
static size_t cache_alloc = 0;   /* allocated array length                 */
static idx_t cache_size = 0;     /* number of terms already computed      */

/* Ensure we have space for and have computed terms up to index n. */
static void ensure_cache(idx_t n)
{
    /* First allocation and seed values */
    if (cache == NULL) {
        cache_alloc = 8;                          /* initial size          */
        cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
        if (!cache) return;                      /* allocation failure – abort silently */

        /* seed the known first four terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)52;

        cache_size = 4;                          /* terms 0..3 are present */
    }

    /* Grow the array as necessary */
    if ((size_t)(n + 1) > cache_alloc) {
        size_t new_alloc = cache_alloc;
        while ((size_t)(n + 1) > new_alloc)
            new_alloc <<= 1;                     /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) return;                       /* allocation failure – abort silently */
        cache = tmp;
        cache_alloc = new_alloc;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* The recurrence is only needed for i ≥ 4, but it is
           harmless to let the loop skip small indices because
           the array already contains the base values. */
        if (i >= 4) {
            seq_t t1 = cache[i - 1] * (seq_t)(7 * i + 2);
            seq_t t2 = cache[i - 2] * (seq_t)(7 * i - 16);
            seq_t t3 = cache[i - 3] * (seq_t)(i - 4);

            /* numerator = t1 - t2 + t3 */
            seq_t num = t1 - t2 + t3;

            /* denominator is i+2 (which is >=6 for i>=4) */
            cache[i] = num / (idx_t)(i + 2);
        }
    }
    cache_size = n + 1;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
