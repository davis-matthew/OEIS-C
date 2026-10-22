/*
 * A376812  G.f.: Sum_{k>=0} x^(k*(k+1)/2) * Product_{j=1..k} (1 + x^j)^2.
 * https://oeis.org/A376812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Sum_{k>=0} x^{k*(k+1)/2} *
 *            Prod_{j=1..k} (1 + x^j)^2
 * Range:     u8 n <= 36, u16 n <= 105, u32 n <= 339, u64 n <= 1180,
 *            u128 n <= 4331, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of stored values (cache_len == last_n+1) */

    /* recompute if requested index is beyond the cached range */
    if (n >= cache_len) {
        size_t N = (size_t)n + 1;
        seq_t *newA   = (seq_t *)calloc(N, sizeof(seq_t));
        seq_t *Q_cur  = (seq_t *)calloc(N, sizeof(seq_t));
        seq_t *Q_next = (seq_t *)calloc(N, sizeof(seq_t));
        Q_cur[0] = 1;                     /* Q_0(x) = 1 */

        idx_t k = 0;
        idx_t T = 0;                       /* T_k = k*(k+1)/2 */

        while (T <= n) {
            /* add current polynomial Q_cur shifted by T to the sequence */
            idx_t shift_limit = n - T;
            for (idx_t i = 0; i <= shift_limit; ++i)
                newA[(size_t)T + (size_t)i] += Q_cur[i];

            /* prepare next k */
            ++k;
            idx_t T_new = T + k;           /* T_{k} = T_{k-1} + k */
            if (T_new > n) break;

            idx_t rem_deg = n - T_new;

            /* zero Q_next up to the degree we will use */
            for (idx_t i = 0; i <= rem_deg; ++i)
                Q_next[i] = 0;

            /* multiply Q_cur by (1 + 2x^k + x^{2k}) */
            for (idx_t i = 0; i <= rem_deg; ++i) {
                seq_t v = Q_cur[i];
                if (!v) continue;
                Q_next[i] += v;                               /* 1 * x^0 */
                if (i + k <= rem_deg)
                    Q_next[i + k] += v * (seq_t)2;           /* 2 * x^k */
                if (i + 2*k <= rem_deg)
                    Q_next[i + 2*k] += v;                    /* 1 * x^{2k} */
            }

            /* swap arrays for next iteration */
            seq_t *tmp = Q_cur;
            Q_cur = Q_next;
            Q_next = tmp;

            T = T_new;
        }

        free(Q_cur);
        free(Q_next);
        free(cache);                           /* free previous cache if any */
        cache = newA;
        cache_len = n + 1;
    }

    return cache[n];
}
