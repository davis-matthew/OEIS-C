/*
 * A318018  Number of nX2 0..1 arrays with every element unequal to 0, 1, 2 or
 *          4 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A318018
 * Recurrence: a(n) = 2*a(n-1)+3*a(n-2)-2*a(n-3)-2*a(n-4)-a(n-6)+a(n-7)
 * Initial values: a(1)=2, a(2)=6, a(3)=14, a(4)=35, a(5)=95,
 *                a(6)=257, a(7)=700
 *
 * The function memoises computed terms so that any order of calls works
 * and the cost is O(n) for the largest n encountered.
 */

#include <stdlib.h>

/* static cache of previously computed terms, indices 1..cache_len */
static seq_t *cache = NULL;        /* pointer to array of a(k) for k=1..cache_len */
static size_t cache_len = 0;       /* number of terms currently stored */

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset starts at 1 – undefined, return 0 */
        return (seq_t)0;

    /* resize cache if needed */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;                     /* new number of terms */
        seq_t *p = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!p) {                                       /* allocation failed */
            free(cache);
            cache = NULL;
            cache_len = 0;
            return (seq_t)0;
        }
        cache = p;

        /* initialise missing terms, using the known initial values */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i < 7) { /* first 7 terms are preset */
                switch ((int)i) {
                    case 0: cache[i] = (seq_t)2;   break;  /* a(1) */
                    case 1: cache[i] = (seq_t)6;   break;  /* a(2) */
                    case 2: cache[i] = (seq_t)14;  break;  /* a(3) */
                    case 3: cache[i] = (seq_t)35;  break;  /* a(4) */
                    case 4: cache[i] = (seq_t)95;  break;  /* a(5) */
                    case 5: cache[i] = (seq_t)257; break;  /* a(6) */
                    case 6: cache[i] = (seq_t)700; break;  /* a(7) */
                }
            } else { /* recurrence for n >= 8 (i >= 7) */
                /* recurrence uses 7 previous terms:
                   a(n-1) -> cache[i-1], a(n-2) -> cache[i-2],
                   a(n-3) -> cache[i-3], a(n-4) -> cache[i-4],
                   a(n-6) -> cache[i-6], a(n-7) -> cache[i-7]   */
                seq_t val = 2 * cache[i - 1]
                          + 3 * cache[i - 2]
                          - 2 * cache[i - 3]
                          - 2 * cache[i - 4]
                          -     cache[i - 6]
                          +     cache[i - 7];
                cache[i] = val;
            }
        }
        cache_len = new_len;
    }

    return cache[n - 1];          /* array is 0‑based, function is 1‑based */
}
