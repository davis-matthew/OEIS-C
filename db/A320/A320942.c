/*
 * A320942  Expansion of Sum_{k>=1} (-1 + Product_{j>=1} (1 + x^(k*j))/(1 -
 *          x^(k*j))).
 * https://oeis.org/A320942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A015128(d) (OEIS A320942)
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for pre‑computed values */
    static seq_t *b = NULL;           /* A015128(d)   */
    static seq_t *a_seq = NULL;      /* final a(n)   */
    static size_t cap = 0;           /* allocated size */

    /* enlarge arrays if required */
    if (n >= cap) {
        idx_t new_cap = (idx_t)((2 * cap) + 10);   /* grow heuristically   */
        if (new_cap < n + 1) new_cap = (size_t)(n + 1);

        /* allocate / reallocate arrays */
        b = (seq_t *)realloc(b, new_cap * sizeof(seq_t));
        if (!a_seq) a_seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else a_seq = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
        if (!b || !a_seq) return (seq_t)0;        /* allocation failed */

        /* initialise new part of arrays */
        for (size_t i = cap; i < new_cap; ++i) {
            b[i] = 0;
            a_seq[i] = 0;
        }

        /* compute sequence b (A015128) up to new_cap-1 */
        b[0] = 1;                     /* coefficient for degree 0 */
        for (idx_t m = 1; m < new_cap; ++m) {
            long long sum = 0;
            for (idx_t k = 1; (k * k) <= m; ++k) {
                /* coefficient of x^{k^2} in 1/theta_4(q)  */
                long long coef = (k % 2 == 0) ? 2 : -2;   /* 2*(-1)^k */
                sum += coef * (long long) b[m - k * k];
            }
            b[m] = (seq_t)(-sum);     /* b[m] = - sum (always non‑negative) */
        }

        /* compute a(n) = sum_{d|n} b[d] for all n < new_cap */
        for (idx_t d = 1; d < new_cap; ++d) {
            for (idx_t m = d; m < new_cap; m += d) {
                a_seq[m] += b[d];
            }
        }

        cap = new_cap;
    }

    return a_seq[n];
}
