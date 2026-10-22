/*
 * A328541  Number of broken 3-diamond partitions of n.
 * https://oeis.org/A328541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of q^n in the generating function G(q) =
 *            Q(-q,q,∞) / [ Q(q,q,∞)^2 * Q(-q^7,q^7,∞) ] Using
 *            Q(-q,q,∞)=∏_{m≥1}(1+q^m), Q(q,q,∞)=∏_{m≥1}(1−q^m),
 *            Q(-q^7,q^7,∞)=∏_{m≥1}(1+q^{7m}) , we get G(q) = Π_{m≥1,
 *            7∤m}(1+q^m) · Π_{m≥1}(1−q^m)^{-2}. The function below evaluates
 *            this series...
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate working arrays of length n+1 */
    seq_t *dp_old = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *dp_new = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!dp_old || !dp_new) return 0;  /* allocation failure */

    /* initial state: only the empty partition */
    for (idx_t i = 0; i <= n; ++i) dp_old[i] = 0;
    dp_old[0] = 1;

    for (idx_t part = 1; part <= n; ++part) {
        /* initialise new array to zero */
        for (idx_t i = 0; i <= n; ++i) dp_new[i] = 0;

        /* process residues modulo part */
        for (idx_t r = 0; r < part && r <= n; ++r) {
            seq_t S = 0;          /* sum of dp_old values */
            seq_t T = 0;          /* weighted sum: sum j*dp_old */
            idx_t idx = r;
            idx_t j   = 0;

            while (idx <= n) {
                seq_t v = dp_old[idx];
                S += v;
                T += v * (seq_t)j;

                seq_t w;
                if (part % 7 == 0) {          /* part divisible by 7 */
                    w = S * (seq_t)(j + 1) - T;
                } else {                       /* other parts */
                    w = S * (seq_t)(2 * j + 1) - T * (seq_t)2;
                }
                dp_new[idx] = w;

                idx += part;
                ++j;
            }
        }

        /* swap dp_old and dp_new for next iteration */
        seq_t *tmp = dp_old;
        dp_old = dp_new;
        dp_new = tmp;
    }

    seq_t result = dp_old[n];

    /* free memory */
    free(dp_old);
    free(dp_new);

    return result;
}
