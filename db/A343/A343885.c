/*
 * A343885  a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-4), with a(1) = a(2) = a(3)
 *          = a(4) = 1.
 * https://oeis.org/A343885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 46, u32 n <= 89, u64 n <= 176,
 *            u128 n <= 350, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A343885:
   a(n) = a(n‑1) - a(n‑2) + a(n‑3) + a(n‑4),  n >= 5
   a(1) = a(2) = a(3) = a(4) = 1
   Offset: 1
*/

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache[0] holds a(1), cache[1] holds a(2), … */
static idx_t  cache_len = 0;    /* number of terms currently stored */
static idx_t  cache_cap = 0;    /* current allocation in terms */

/* Grow the cache array to hold at least `needed` terms */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap) return;

    idx_t new_cap = (cache_cap == 0) ? 4 : cache_cap;
    while (new_cap < needed) new_cap <<= 1;          /* double until enough */

    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) {   /* allocation failed – fatal error */
        abort();
    }
    cache     = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but safe for n < 1 */

    /* compute missing terms up to n */
    if (n > cache_len) {
        ensure_capacity(n);
        for (idx_t i = cache_len; i < n; ++i) {           /* i = 0 → a(1) */
            seq_t val;
            if (i < 4) {                                 /* a(1)…a(4) */
                val = (seq_t)1;
            } else {
                /* recurrence: a(n) = a(n‑1) - a(n‑2) + a(n‑3) + a(n‑4) */
                val = cache[i - 1] - cache[i - 2]
                      + cache[i - 3] + cache[i - 4];
            }
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];   /* cache[0] holds a(1) */
}
