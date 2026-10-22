/*
 * A353896  Expansion of e.g.f. exp( (x * (exp(x) - 1))^4 / 576 ).
 * https://oeis.org/A353896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/8)} (4k)! *
 *            Stirling2(n-4k,4k)/(576^k * k! * (n-4k)!). The computation uses
 *            the exponential Bell‑polynomial recurrence a_0 = 1, a_m =
 *            Σ_{k=1..m} C(m-1,k-1) * f_k * a_{m-k}, where f_k =
 *            (k*(k-1)*(k-2)*(k-3)/24) * S(k-4,4) and S(m,4) = (4^m - 4*3^m +
 *            6*2^m - 4)/24
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 24, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    /* Allocate temporary arrays for f and a */
    seq_t *f = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *A = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!f || !A) {
        /* In case of allocation failure, return 0 */
        if (f) free(f);
        if (A) free(A);
        return 0;
    }

    /* Initialize f with zeros */
    for (size_t i = 0; i <= N; ++i) f[i] = 0;

    /* Precompute powers 2^e, 3^e, 4^e for e = k-4 */
    seq_t pow2 = 1, pow3 = 1, pow4 = 1;   /* values for e = 0 initially */
    if (N >= 4) {
        for (size_t e = 0; e <= N - 4; ++e) {
            size_t k = e + 4;   /* current k */
            /* S(k-4,4) = (4^(k-4) - 4*3^(k-4) + 6*2^(k-4) - 4)/24 */
            seq_t tmp = pow4;
            seq_t tmp2 = pow3; tmp2 *= 4; tmp -= tmp2;
            tmp2 = pow2; tmp2 *= 6; tmp += tmp2;
            tmp2 = 1; tmp2 *= 4; tmp -= tmp2;
            seq_t S = tmp / 24;

            if (k >= 8) {
                seq_t coeff = (seq_t)k * (k - 1);
                coeff *= (k - 2);
                coeff *= (k - 3);
                coeff /= 24;               /* integer division is exact */
                f[k] = coeff * S;
            } else {
                f[k] = 0;   /* f_k is zero for k < 8 */
            }

            /* Update powers for next e */
            pow2 *= 2;
            pow3 *= 3;
            pow4 *= 4;
        }
    }

    /* Bell‑polynomial recurrence to compute a[n] */
    A[0] = 1;   /* a_0 = 1 */
    for (size_t m = 1; m <= N; ++m) {
        seq_t sum = 0;
        seq_t binom = 1;   /* C(m-1,0) */
        for (size_t k = 1; k <= m; ++k) {
            if (k > 1) {
                seq_t mult = (seq_t)(m - (k - 1));
                binom = binom * mult;
                binom = binom / (k - 1);
            }
            if (k >= 8 && k <= m) {
                seq_t prod = A[m - k];
                prod = prod * f[k];
                prod = prod * binom;
                sum += prod;
            }
        }
        A[m] = sum;
    }

    seq_t result = A[N];

    /* Free allocated memory */
    free(f);
    free(A);

    return result;
}
