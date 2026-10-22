/*
 * A373510  Expansion of 1/(1 - x/(1 - 25*x^5)^(1/5)).
 * https://oeis.org/A373510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   Let neg_g[k] = -g[k] where g is defined by g[0]=1,
   g[5*m] = - neg_g[5*m] = - neg_g[5(m-1)] * (5*(m-1)-1)*5/(m) for m≥2,
   neg_g[5] = 5.
   Then H[0] = 1, H[n] = Σ_{k=5, k≤n} neg_g[k] * H[n-k]   (k step 5).
   Finally a[0] = 1, and for n>0:
        a[n] = Σ_{i=0}^{n-1} H[n-1-i] * a[i].
   The above recurrence reproduces A373510. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *A = NULL;
    static seq_t *H = NULL;
    static seq_t *neg_g = NULL;
    static idx_t cache_size = 0;          /* number of elements currently computed (size) */

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        A = (seq_t*)realloc(A, new_size * sizeof(seq_t));
        H = (seq_t*)realloc(H, new_size * sizeof(seq_t));
        neg_g = (seq_t*)realloc(neg_g, new_size * sizeof(seq_t));

        /* initialise new elements */
        if (cache_size == 0) {   /* first time */
            A[0] = (seq_t)1;
            H[0] = (seq_t)1;
            neg_g[0] = (seq_t)0;
            cache_size = 1;
        }

        /* compute missing values */
        for (idx_t i = cache_size; i < new_size; ++i) {
            /* compute neg_g[i] */
            if (i % 5 == 0 && i >= 5) {
                if (i == 5) {
                    neg_g[i] = (seq_t)5;
                } else {
                    idx_t m = i / 5;
                    idx_t t = m - 1;               /* t = m-1 ≥ 1 */
                    seq_t prev = neg_g[5 * t];
                    seq_t term = prev * (seq_t)(5 * t - 1);
                    term = term * (seq_t)5;
                    term = term / (seq_t)(t + 1);
                    neg_g[i] = term;
                }
            } else {
                neg_g[i] = (seq_t)0;
            }

            /* compute H[i] */
            seq_t sumH = (seq_t)0;
            for (idx_t k = 5; k <= i; k += 5) {
                /* neg_g[k] is non‑zero only when k≥5 */
                sumH = sumH + neg_g[k] * H[i - k];
            }
            H[i] = sumH;

            /* compute A[i] */
            seq_t sumA = (seq_t)0;
            for (idx_t j = 0; j <= i - 1; ++j) {
                sumA = sumA + H[i - 1 - j] * A[j];
            }
            A[i] = sumA;
        }

        cache_size = new_size;
    }

    return A[n];
}
