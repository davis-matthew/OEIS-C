/*
 * A289082  Real parts of the recursive sequence a(n+2) = Sum_{k=0..n}
 *          binomial(n,k)*a(k)*a(n+1-k), with a(0)=1, a(1)=i.
 * https://oeis.org/A289082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=0; the complex sequence c(0)=1, c(1)=i satisfies
 *            c(n+2)=∑_{k=0}^n binom(n,k)·c(k)·c(n+1-k). a(n) is the real part
 *            of c(n)
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 15, i64 n <= 24, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Handle the two initial values directly. */
    if (n == 0) return 1;
    if (n == 1) return 0;

#ifdef __cplusplus
    std::vector<seq_t> R(n + 1), I(n + 1);
    R[0] = 1;  I[0] = 0;
    R[1] = 0;  I[1] = 1;
#else
    seq_t *R = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *I = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!R || !I) return 0;          /* insufficient memory */
    R[0] = 1;  I[0] = 0;
    R[1] = 0;  I[1] = 1;
#endif

    /* Compute all terms up to n using the recurrence. */
    for (idx_t m = 2; m <= n; ++m) {
        idx_t kmax = m - 2;          /* k runs from 0 to m-2 */
        seq_t r_sum = 0;
        seq_t i_sum = 0;
        seq_t binom = 1;             /* binom(m-2,0) */

        for (idx_t k = 0; k <= kmax; ++k) {
            seq_t rk = R[k];
            seq_t rg = R[m - 1 - k];
            seq_t ik = I[k];
            seq_t ig = I[m - 1 - k];

            seq_t termR = (rk * rg) - (ik * ig);
            seq_t termI = (rk * ig) + (ik * rg);

            r_sum += binom * termR;
            i_sum += binom * termI;

            /* Update binom to binom(m-2, k+1) in next iteration. */
            if (k < kmax) {
                binom = binom * (kmax - k) / (k + 1);
            }
        }

        R[m] = r_sum;
        I[m] = i_sum;
    }

#ifdef __cplusplus
    return R[n];
#else
    seq_t result = R[n];
    free(R);
    free(I);
    return result;
#endif
}
