/*
 * A297655  Number of 2Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 2 or 3 neighboring 1s.
 * https://oeis.org/A297655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + 13*a(n-3) + 6*a(n-4) + 12*a(n-5) +
 *            12*a(n-6) (for n ≥ 7) with a(1)=1, a(2)=10, a(3)=43, a(4)=140,
 *            a(5)=494, a(6)=1845
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the nth term of OEIS A297655.  */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* one‑based array of computed terms   */
static idx_t  cache_last = 0;    /* largest index computed so far      */

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* the sequence is defined only for n ≥ 1 */

    /* Ensure the cache is large enough.  */
    if (cache == NULL) {
        cache = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        cache_last = 0;
    } else if ((idx_t)(cache_last + 1) <= n) {
        /* Grow the cache if necessary.  */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache = tmp;
    }

    /* Compute missing terms up to n.  */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        switch (i) {
            case 1:  cache[i] = (seq_t)1;          break;
            case 2:  cache[i] = (seq_t)10;         break;
            case 3:  cache[i] = (seq_t)43;         break;
            case 4:  cache[i] = (seq_t)140;        break;
            case 5:  cache[i] = (seq_t)494;        break;
            case 6:  cache[i] = (seq_t)1845;       break;
            default: {
                seq_t a1 = cache[i-1];
                seq_t a2 = cache[i-2];
                seq_t a3 = cache[i-3];
                seq_t a4 = cache[i-4];
                seq_t a5 = cache[i-5];
                seq_t a6 = cache[i-6];

                /* Compute the linear recurrence in a safe order to avoid
                   unnecessary unsigned underflows on common 64‑bit builds. */
                seq_t pos = (seq_t)3 * a1
                          + (seq_t)13 * a3
                          + (seq_t)6  * a4
                          + (seq_t)12 * a5
                          + (seq_t)12 * a6;

                cache[i] = pos - (seq_t)2 * a2;
            } break;
        }
    }
    cache_last = n;
    return cache[n];
}
