/*
 * A269534  Number of length-n 0..5 arrays with no repeated value differing
 *          from the previous repeated value by other than one.
 * https://oeis.org/A269534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A269534: number of length‑n 0..5 arrays with no repeated value
   differing from the previous repeated value by any amount other than one.
   
   Recurrence (valid for n ≥ 8):
     a(n) = 17*a(n‑1) - 92*a(n‑2) + 96*a(n‑3) + 493*a(n‑4)
           - 533*a(n‑5) - 1521*a(n‑6) - 695*a(n‑7)
   Initial values:
     a(1)=6, a(2)=36, a(3)=210, a(4)=1210, a(5)=6890,
     a(6)=38878, a(7)=217714 */
#include <stdlib.h>

static seq_t *cache = NULL;     /* array 0‑based, index 0 unused */
static size_t cache_size = 0;   /* allocated size of cache (in elements) */
static size_t last_used = 0;    /* maximum n that has been computed */

/* Compute a(n) (1‑indexed) for any n ≥ 1 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n ≤ 0 */
    size_t idx = (size_t)n;                  /* zero‑based array index */

    /* allocate or grow the cache if needed */
    if (idx + 1 > cache_size) {
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                   /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* initialise first seven terms if they haven't been set yet */
    static const seq_t init[8] = { 0,
                                   (seq_t)6,     /* a(1) */
                                   (seq_t)36,    /* a(2) */
                                   (seq_t)210,   /* a(3) */
                                   (seq_t)1210,  /* a(4) */
                                   (seq_t)6890,  /* a(5) */
                                   (seq_t)38878, /* a(6) */
                                   (seq_t)217714 /* a(7) */
                                 };
    if (last_used < 7) {
        for (size_t i = 1; i <= 7 && i <= idx; ++i) {
            if (i > last_used) cache[i] = init[i];
        }
    }

    /* compute missing terms up to n */
    if (idx > last_used) {
        for (size_t i = last_used + 1; i <= idx; ++i) {
            if (i <= 7) {
                cache[i] = init[i];
            } else {
                cache[i] =  cache[i-1] * (seq_t)17
                           - cache[i-2] * (seq_t)92
                           + cache[i-3] * (seq_t)96
                           + cache[i-4] * (seq_t)493
                           - cache[i-5] * (seq_t)533
                           - cache[i-6] * (seq_t)1521
                           - cache[i-7] * (seq_t)695;
            }
        }
        last_used = idx;
    }

    return cache[idx];
}
