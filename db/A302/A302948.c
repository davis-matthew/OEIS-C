/*
 * A302948  Number of nX3 0..1 arrays with every element equal to 1, 2, 3, 4
 *          or 5 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 15*a(n-2) + 13*a(n-3) - 2*a(n-4) - 19*a(n-5) -
 *            3*a(n-6) + 4*a(n-8) (empirical) Initial values: a(1)=1, a(2)=15,
 *            a(3)=86, a(4)=587, a(5)=3815, a(6)=25131, a(7)=164916,
 *            a(8)=1083375 (Computed via recurrence for n≥9.)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array [0..n], 0 unused               */
static idx_t computed = 0;       /* highest n that has already been computed */
static size_t cap = 0;           /* allocated size (elements)                 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                                  /* offset 1 */

    /* Ensure size n+1 to use 1‑based indexing */
    size_t needed = (size_t)n + 1U;
    if (cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return 0;                               /* allocation failed */
        cache = tmp;
        cap   = needed;
    }

    if (computed < n) {                                    /* compute missing terms */
        for (idx_t i = computed + 1; i <= n; ++i) {
            switch (i) {
                case 1: cache[i] = (seq_t)1;   break;
                case 2: cache[i] = (seq_t)15;  break;
                case 3: cache[i] = (seq_t)86;  break;
                case 4: cache[i] = (seq_t)587; break;
                case 5: cache[i] = (seq_t)3815; break;
                case 6: cache[i] = (seq_t)25131; break;
                case 7: cache[i] = (seq_t)164916; break;
                case 8: cache[i] = (seq_t)1083375; break;
                default: {
                    seq_t v = (seq_t)4 * cache[i-1];
                    v += (seq_t)15 * cache[i-2];
                    v += (seq_t)13 * cache[i-3];
                    v -= (seq_t)2 * cache[i-4];
                    v -= (seq_t)19 * cache[i-5];
                    v -= (seq_t)3 * cache[i-6];
                    v += (seq_t)4 * cache[i-8];
                    cache[i] = v;
                } break;
            }
        }
        computed = n;
    }
    return cache[n];
}
