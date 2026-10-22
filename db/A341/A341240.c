/*
 * A341240  a(n) = 4*a(n-1) - 2*a(n-2) + a(n-3) - 4*a(n-4) + 2*a(n-5) for n >=
 *          7, where a(1) = 1, a(2) = 2, a(3) = 4, a(4) = 12, a(5) = 38, a(6)
 *          = 127.
 * https://oeis.org/A341240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + a(n-3) - 4*a(n-4) + 2*a(n-5) for n
 *            >= 7, with a(1)=1, a(2)=2, a(3)=4, a(4)=12, a(5)=38, a(6)=127
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   // malloc, realloc, abort

seq_t a(idx_t n)
{
    // Memoization cache
    static seq_t *cache = NULL;          // 0‑indexed: cache[i] == a(i+1)
    static idx_t cache_filled = 0;       // number of terms stored

    if (n == 0) return (seq_t)0; /* behaviour below offset is irrelevant */

    size_t target = (size_t)n;          // limit of growth (fits because n <= ~10000)
    size_t old_filled = (size_t)cache_filled;

    /* Ensure capacity */
    if (cache == NULL) {
        cache = (seq_t *)malloc(target * sizeof(seq_t));
        if (cache == NULL) abort();
    } else if (old_filled < target) {
        seq_t *tmp = (seq_t *)realloc(cache, target * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
    }

    /* Compute missing terms */
    for (size_t i = old_filled; i < target; ++i) {
        seq_t val;
        switch ((idx_t)i) {                      /* initial values (i = 0 → a(1)) */
            case 0: val = (seq_t)1;  break;
            case 1: val = (seq_t)2;  break;
            case 2: val = (seq_t)4;  break;
            case 3: val = (seq_t)12; break;
            case 4: val = (seq_t)38; break;
            case 5: val = (seq_t)127;break;
            default: {
                seq_t a1 = cache[i-1]; /* a(i)   */
                seq_t a2 = cache[i-2]; /* a(i-1) */
                seq_t a3 = cache[i-3]; /* a(i-2) */
                seq_t a4 = cache[i-4]; /* a(i-3) */
                seq_t a5 = cache[i-5]; /* a(i-4) */
                val = (seq_t)4 * a1 - (seq_t)2 * a2 + a3
                    - (seq_t)4 * a4 + (seq_t)2 * a5;
                break;
            }
        }
        cache[i] = val;
        cache_filled++;   /* grow the filled count */
    }

    return cache[target-1];
}
