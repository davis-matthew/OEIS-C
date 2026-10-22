/*
 * A365989  Expansion of e.g.f. exp( Sum_{k>=0} x^(5*k+4) / (5*k+4) ).
 * https://oeis.org/A365989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (n-1)! * Sum_{k=0..⌊(n-4)/5⌋}
 *            a(n-5*k-4)/(n-5*k-4)!
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 13, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* highest index computed so far */

    /* Allocate / extend cache on first request or when n is larger */
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;     /* allocation failure */
        cache[0] = (seq_t)1;
        cached = 0;
    } else if (n > cached) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;     /* allocation failure */
    }

    /* Compute values up to n */
    for (idx_t i = cached + 1; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
            continue;
        }
        if (i < 4) {
            cache[i] = (seq_t)0;
            continue;
        }

        seq_t sum = 0;
        idx_t maxk = (i - 4) / 5;         /* floor((i-4)/5) */
        idx_t m_min = i - 5 * maxk - 4;   /* smallest m in the sum */

        /* Compute product P = ∏_{t=m_min+1}^{i-1} t  */
        seq_t P = 1;
        for (idx_t t = m_min + 1; t <= i - 1; ++t) {
            P *= (seq_t)t;
        }

        /* Iterate k from maxk down to 0, updating P each step */
        for (idx_t k = maxk; ; --k) {
            idx_t m = i - 5 * k - 4;          /* current m */
            sum += cache[m] * P;

            if (k == 0) break;               /* finished loop */

            /* Update P for next larger m: divide by (m+1)*(m+2)*(m+3)*(m+4)*(m+5) */
            for (idx_t d = 1; d <= 5; ++d) {
                P /= (seq_t)(m + d);
            }
        }

        cache[i] = sum;
    }

    cached = n;
    return cache[n];
}
