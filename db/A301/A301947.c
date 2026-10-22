/*
 * A301947  Number of nX4 0..1 arrays with every element equal to 1, 2 or 3
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 8*a(n-2) - 14*a(n-3) + 4*a(n-4) + 4*a(n-5) -
 *            a(n-6) with a(1)=2, a(2)=16, a(3)=123, a(4)=947, a(5)=7326,
 *            a(6)=56710
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    static seq_t *cache = NULL;          /* cached terms a(1) … a(cached) */
    static size_t cached = 0;            /* number of terms currently cached */

    /* ensure sufficient storage */
    if ((size_t)n > cached) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure */
        cache = tmp;
    }

    /* initialise missing base terms */
    if (cached < 6) {
        const seq_t base[6] = {
            (seq_t)2,
            (seq_t)16,
            (seq_t)123,
            (seq_t)947,
            (seq_t)7326,
            (seq_t)56710
        };
        size_t upto = 6;
        if ((size_t)n < 6) upto = (size_t)n;
        for (size_t i = cached; i < upto; ++i)
            cache[i] = base[i];
        cached = upto;
    }

    /* compute any remaining terms using the recurrence */
    size_t start = (cached > 6) ? cached : 6; /* next index to compute (0‑based) */
    for (size_t i = start; i < (size_t)n; ++i) {
        cache[i] =
            (seq_t)((seq_t)9  * cache[i-1] -
                    (seq_t)8  * cache[i-2] -
                    (seq_t)14 * cache[i-3] +
                    (seq_t)4  * cache[i-4] +
                    (seq_t)4  * cache[i-5] -
                    cache[i-6]);
    }
    cached = (size_t)n;

    return cache[n-1];
}
