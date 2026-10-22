/*
 * A400811  Size of Type A palindromes of star triples in the Spectre
 *          aperiodic monotile tiling.
 * https://oeis.org/A400811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = 3*a(n-1) + 15*a(n-2) + a(n-3) for n > 3 with initial
 *            values a(1)=2, a(2)=10, a(3)=60
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for memoisation */
static seq_t *cache = NULL;      /* array of computed terms (0‑based) */
static size_t cache_cap = 0;     /* allocated capacity of the cache      */
static size_t cache_size = 0;    /* number of terms currently stored    */

/* Compute the nth term (1‑based) of sequence A400811 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* behaviour for n < 1 is undefined */

    size_t idx = (size_t)n;       /* use size_t for array indices */

    /* Allocate or grow the cache if necessary */
    if (cache_cap < idx) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < idx) new_cap = idx;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* In a real program handle allocation failure; here we assume success. */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to idx */
    if (cache_size < idx) {
        for (size_t i = cache_size + 1; i <= idx; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)2;
            } else if (i == 2) {
                val = (seq_t)10;
            } else if (i == 3) {
                val = (seq_t)60;
            } else {
                /* Recurrence: a(i) = 3*a(i-1) + 15*a(i-2) + a(i-3) */
                seq_t a1 = cache[i - 2]; /* a(i-1)  */
                seq_t a2 = cache[i - 3]; /* a(i-2)  */
                seq_t a3 = cache[i - 4]; /* a(i-3)  */
                val = a1 * (seq_t)3 + a2 * (seq_t)15 + a3;
            }
            cache[i - 1] = val;
        }
        cache_size = idx;
    }

    return cache[idx - 1];
}
