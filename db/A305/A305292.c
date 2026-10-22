/*
 * A305292  Numbers k such that k-1 is a square and k+1 is a triangular
 *          number.
 * https://oeis.org/A305292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*a(n-2) - 35*a(n-4) + a(n-6) (n >= 7) Initial values:
 *            a(1) = 2 a(2) = 5 a(3) = 65 a(4) = 170 a(5) = 2210 a(6) = 5777
 *            The function uses memoisation to support arbitrary call order
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed terms (0‑based) */
static size_t cached = 0;     /* number of elements in 'cache' */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* behaviour below offset is irrelevant */

    size_t idx = (size_t)n - 1;   /* 0‑based index */

    if (idx >= cached) {
        /* need to extend the cache up to 'idx' */
        size_t newcap = idx + 1;
        if (!cache) {
            cache = (seq_t *)malloc(newcap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        }
        /* initialise new entries */
        for (size_t i = cached; i <= idx; ++i) {
            if (i < 6) {
                /* hard‑coded initial values */
                switch (i) {
                    case 0: cache[i] = (seq_t)2;   break;
                    case 1: cache[i] = (seq_t)5;   break;
                    case 2: cache[i] = (seq_t)65;  break;
                    case 3: cache[i] = (seq_t)170; break;
                    case 4: cache[i] = (seq_t)2210; break;
                    case 5: cache[i] = (seq_t)5777; break;
                }
            } else {
                /* recurrence: a(n) = 35*a(n-2) - 35*a(n-4) + a(n-6) */
                seq_t term = 35 * cache[i - 2];
                term = term - 35 * cache[i - 4];
                term = term + cache[i - 6];
                cache[i] = term;
            }
        }
        cached = newcap;
    }
    return cache[idx];
}
