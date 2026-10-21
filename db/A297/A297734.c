/*
 * A297734  Number of 2Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2 or 4 neighboring 1s.
 * https://oeis.org/A297734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 3*a(n-3) + 6*a(n-4) - 6*a(n-5) with
 *            initial values a(1)=1, a(2)=2, a(3)=8, a(4)=15, a(5)=32
 *            (sequence A297734)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* no defined index 0 for this sequence, but allow caller convenience */
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;   /* cache of computed terms (1‑based index) */
    static size_t cap   = 0;      /* allocated size, including index 0 */
    static idx_t  last  = 0;      /* highest index computed so far */

    /* ensure capacity for at least max(6, n+1) elements */
    size_t needed = (size_t)n + 1;
    if (needed < 6) needed = 6;
    if (cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) {               /* allocation failed: return 0 sentinel */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = needed;
    }

    /* initialize first five terms on first use */
    if (last < 5) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)15;
        cache[5] = (seq_t)32;
        last = 5;
    }

    if (n <= last)
        return cache[n];

    /* compute missing terms using recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = (seq_t)2 * cache[i-1] + cache[i-2];
        term -= (seq_t)3 * cache[i-3];
        term += (seq_t)6 * cache[i-4];
        term -= (seq_t)6 * cache[i-5];
        cache[i] = term;
    }
    last = n;
    return cache[n];
}
