/*
 * A321026  Expansion of Product_{k>0} theta_3(q^(2*k-1))/theta_3(q^(2*k)),
 *          where theta_3() is the Jacobi theta function.
 * https://oeis.org/A321026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   product_{k>0} theta_3(q^(2k-1)) / theta_3(q^(2k)) where
 *            theta_3(q)=sum_{m∈Z} q^{m^2}
 * Range:     i8 n <= 13, i16 n <= 37, i32 n <= 107, i64 n <= 329,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* compute product up to exponent n */
    const idx_t N = n;
    const idx_t Kmax = (N + 1) / 2;          /* largest k that can influence coeffs ≤ N */

    /* allocate coefficient arrays */
    seq_t *S = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *T = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!S || !T) {
        /* abort on allocation failure */
        return (seq_t)0;
    }

    S[0] = (seq_t)1;   /* start with 1 */

    for (idx_t k = 1; k <= Kmax; ++k) {
        /* exponents for numerator: (2k-1)*m^2  */
        idx_t d_num = 2 * k - 1;
        /* exponents for denominator: 2k*m^2  */
        idx_t d_den = 2 * k;

        /* build arrays of exponents */
        /* maximum possible exponents are ≤ N  */
        idx_t exp_num[128], sz_num = 0;
        for (idx_t m = 1;; ++m) {
            idx_t e = d_num * m * m;
            if (e > N) break;
            exp_num[sz_num++] = e;
        }

        idx_t exp_den[128], sz_den = 0;
        for (idx_t m = 1;; ++m) {
            idx_t e = d_den * m * m;
            if (e > N) break;
            exp_den[sz_den++] = e;
        }

        /* ------------  Multiply S by numerator  ------------ */
        /* T = S  + 2 * sum_{e in exp_num} shift(S, e)  */
        for (idx_t i = 0; i <= N; ++i) T[i] = S[i];
        for (idx_t idx = 0; idx < sz_num; ++idx) {
            idx_t shift = exp_num[idx];
            for (idx_t i = 0; i + shift <= N; ++i) {
                T[i + shift] += (seq_t)2 * S[i];
            }
        }

        /* ------------  Divide by denominator  ------------ */
        /* Solve (1 + 2*sum_{e in exp_den} q^e) * S = T  */
        /* Since constant term is 1, we can compute in increasing order of e. */
        for (idx_t e = 0; e <= N; ++e) {
            seq_t sub = (seq_t)0;
            for (idx_t idx = 0; idx < sz_den; ++idx) {
                idx_t shift = exp_den[idx];
                if (shift > e) break;
                sub += (seq_t)2 * S[e - shift];
            }
            S[e] = T[e] - sub;
        }
    }

    seq_t result = S[N];
    free(T);
    free(S);
    return result;
}
