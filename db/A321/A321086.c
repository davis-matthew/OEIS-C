/*
 * A321086  O.g.f. A(x) satisfies: [x^n] exp(n*A(x)) * (1 - n*x - n*x^2) = 0,
 *          for n > 0.
 * https://oeis.org/A321086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A321086.
   Recurrence: a(1) = 1, a(2) = 2, for n > 2
   a(n) = Σ_{k=1}^{n-1} (n-k) * a(k) * a(n-k).   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; this value is unused */

    /* static cache: cache[i] holds a(i+1) for 0 <= i < len   */
    static seq_t *cache = NULL;
    static idx_t  len   = 0;       /* number of terms currently stored   */
    static idx_t  cap   = 0;       /* allocated capacity                  */

    /* Expand storage if necessary */
    if (n > cap) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failed */
        cache = tmp;
        cap   = new_cap;

        /* Initialise first two terms if they have not been set yet */
        if (len == 0) {
            cache[0] = 1;                   /* a(1) */
            len = 1;
            if (n >= 2) {
                cache[1] = 2;               /* a(2) */
                len = 2;
            }
        } else if (len == 1 && n >= 2) {
            cache[1] = 2;                   /* a(2) */
            len = 2;
        }

        /* Compute any new terms up to n */
        for (idx_t m = len + 1; m <= n; ++m) {
            seq_t sum = 0;
            for (idx_t k = 1; k < m; ++k) {
                sum += (seq_t)(m - k) * cache[k - 1] * cache[m - k - 1];
            }
            cache[m - 1] = sum;
        }
        len = n;
    }

    return cache[n - 1];
}
