/*
 * A246833  Expansion of psi(-x)^2 * psi(x^4) in powers of x where psi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A246833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 150, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A246833: 
   a(n) = coefficient of x^n in ψ(-x)^2 * ψ(x^4), 
   where ψ(q) = Σ_{r≥0} q^{r(r+1)/2}. 
   The coefficient can be written as
   a(n) = Σ_{m1,m2,k≥0} (-1)^{T_{m1}+T_{m2}}
          if T_{m1}+T_{m2}+2k(k+1)=n,
          where T_m = m(m+1)/2. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;

    /* maximum index for triangular numbers (T_m <= n) */
    idx_t max_m = 0;
    while ((seq_t)max_m * (max_m + 1) / 2 <= n) ++max_m;
    if (max_m > 0) --max_m;            /* last valid m */

    /* triangular numbers array */
    size_t msize = (size_t)(max_m + 1);
    idx_t *T = (idx_t*)malloc(msize * sizeof(idx_t));
    for (idx_t i = 0; i <= max_m; ++i) {
        T[i] = i * (i + 1) / 2;
    }

    /* maximum index for k in 2k(k+1) <= n */
    idx_t max_k = 0;
    while ((seq_t)2 * max_k * (max_k + 1) <= n) ++max_k;
    if (max_k > 0) --max_k;

    /* precompute 2k(k+1) values */
    size_t ksize = (size_t)(max_k + 1);
    idx_t *K = (idx_t*)malloc(ksize * sizeof(idx_t));
    for (idx_t k = 0; k <= max_k; ++k) {
        K[k] = 2 * k * (k + 1);
    }

    /* Core summation */
    for (idx_t m1 = 0; m1 <= max_m; ++m1) {
        seq_t t1 = T[m1];
        int sign1 = (t1 & 1) ? -1 : 1;  /* (-1)^{T_m1} */

        for (idx_t m2 = 0; m2 <= max_m; ++m2) {
            seq_t t2 = T[m2];
            seq_t s12 = t1 + t2;
            if (s12 > n) break;          /* further m2 will only increase s12 */

            int sign = sign1 * ((t2 & 1) ? -1 : 1);   /* (-1)^{T_m1+T_m2} */
            seq_t remaining = n - s12;

            for (idx_t k = 0; k <= max_k; ++k) {
                seq_t t3 = K[k];
                if (t3 > remaining) break;
                if (t3 == remaining) sum += sign;
            }
        }
    }

    free(T);
    free(K);
    return sum;
}
