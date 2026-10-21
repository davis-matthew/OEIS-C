/*
 * A261116  Pairs of integers (a,b) such a^2 + (a+1)^2 = b^2.
 * https://oeis.org/A261116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = 7*a(n-2) - 7*a(n-4) + a(n-6) for n>6 Initial values:
 *            a(1)=0, a(2)=1, a(3)=3, a(4)=5, a(5)=20, a(6)=29
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 52, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms.
       cache[0] holds a(1), cache[1] holds a(2), etc. */
    static seq_t *cache = NULL;
    static size_t cached = 0;   /* number of terms currently stored */

    /* Base values for the first six terms */
    static const seq_t base[6] = {0, 1, 3, 5, 20, 29};

    if (n == 0)
        return 0; /* undefined for n<1, treat as 0 */

    /* Extend cache if necessary */
    if ((size_t)n > cached) {
        size_t newcap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            abort();          /* allocation failed */

        cache = tmp;

        /* Compute new terms */
        for (size_t i = cached; i < newcap; ++i) {
            if (i < 6) {
                cache[i] = base[i];
            } else {
                cache[i] = (seq_t)(7) * cache[i-2]
                           - (seq_t)(7) * cache[i-4]
                           + cache[i-6];
            }
        }

        cached = newcap;
    }

    return cache[n - 1]; /* convert 1‑based index to 0‑based array index */
}
