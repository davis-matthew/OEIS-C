/*
 * A293300  E.g.f.: Product_{m>0} 1/(1 + x^m).
 * https://oeis.org/A293300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * coefficient of x^n in ∏_{m>0} 1/(1 + x^m)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*  Computes a(n) for n >= 0  */
seq_t a(idx_t n)
{
    /* static caches */
    static seq_t *coeff = NULL;   /* a(n)/n!  */
    static seq_t *fact   = NULL;   /* n!      */
    static int8_t *phi   = NULL;   /* φ(n)   = coefficient of x^n in ∏(1–x^m) */
    static int8_t *qVal  = NULL;   /* q(n)   = coefficient of x^n in ∏(1–x^{2m}) */
    static idx_t *pent   = NULL;   /* list of pentagonal numbers g(k)=k(3k±1)/2 */
    static size_t pent_cnt = 0;
    static size_t max_n = 0;       /* largest index already computed */

    /* initialise if first call */
    if (coeff == NULL) {
        /* allocate minimal size (1) */
        coeff  = (seq_t*)realloc(coeff, 1 * sizeof(seq_t));
        fact   = (seq_t*)realloc(fact, 1 * sizeof(seq_t));
        phi    = (int8_t*)realloc(phi, 1 * sizeof(int8_t));
        qVal   = (int8_t*)realloc(qVal, 1 * sizeof(int8_t));
        pent   = (idx_t*)realloc(pent, 1 * sizeof(idx_t));

        coeff[0] = (seq_t)1;
        fact[0]  = (seq_t)1;
        phi[0]   = 1;
        qVal[0]  = 1;
        pent[0]  = 0;  /* not used */
        pent_cnt = 0;
        max_n = 0;
    }

    /* extend caches if needed */
    if (n > max_n) {
        size_t new_max = n;

        coeff = (seq_t*)realloc(coeff, (new_max+1)*sizeof(seq_t));
        fact  = (seq_t*)realloc(fact,  (new_max+1)*sizeof(seq_t));
        phi   = (int8_t*)realloc(phi,  (new_max+1)*sizeof(int8_t));
        qVal  = (int8_t*)realloc(qVal, (new_max+1)*sizeof(int8_t));

        /* initialise new entries */
        for (size_t i = max_n+1; i <= new_max; ++i) {
            coeff[i] = 0;
            fact[i]  = 0;
            phi[i]   = 0;
            qVal[i]  = 0;
        }

        /* rebuild list of pentagonal numbers up to new_max */
        free(pent);
        pent   = (idx_t*)malloc((new_max+1)*sizeof(idx_t));
        pent_cnt = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t val1 = k * (3*k - 1) / 2;
            if (val1 > new_max) break;
            phi[val1] = (k & 1) ? -1 : 1;
            pent[pent_cnt++] = val1;

            idx_t val2 = k * (3*k + 1) / 2;
            if (val2 > new_max) continue;
            phi[val2] = (k & 1) ? -1 : 1;
            pent[pent_cnt++] = val2;
        }
        phi[0] = 1;   /* φ(0) = 1 */

        /* q(n) = φ(n/2) if n even, otherwise 0, but q(0)=1 */
        qVal[0] = 1;
        for (idx_t i = 1; i <= new_max; ++i) {
            qVal[i] = (i & 1) ? 0 : phi[i/2];
        }

        /* factorials */
        if (max_n == 0) fact[0] = (seq_t)1;
        for (idx_t i = max_n+1; i <= new_max; ++i) {
            fact[i] = fact[i-1] * (seq_t)i;           /* multiplication */
        }

        /* coefficients a(n)/n!  (c[n]) */
        if (max_n == 0) coeff[0] = (seq_t)1;

        for (idx_t i = max_n+1; i <= new_max; ++i) {
            seq_t sum = 0;
            for (size_t j = 0; j < pent_cnt && 2*pent[j] <= i; ++j) {
                int8_t sign = phi[pent[j]];
                idx_t k = i - 2*pent[j];
                sum += (seq_t)sign * coeff[k];
            }
            coeff[i] = (seq_t)phi[i] - sum;
        }

        max_n = new_max;
    }

    return fact[n] * coeff[n];
}
