/*
 * A257290  Number of 3-Motzkin paths of length n with no level steps at even
 *          level.
 * https://oeis.org/A257290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..floor(n/2)} 3^(n-2i) * Catalan(i) *
 *            binomial(n-i-1, i-1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where Catalan(0)=1, binomial(m,k)=0 if k<0 or k>m.                      */

#include <stdint.h>
#include "../utils/pow_int.h"   /* big integer exponentiation */

seq_t a(idx_t n) {
    /* Special cases for small n */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* floor(n/2) determines how many terms to evaluate */
    idx_t half = n / 2;

    seq_t sum = 0;

    /* 3^(n-2) will be the first power used (i = 1) */
    seq_t pow3_cur = pow_int((seq_t)3, n - 2);

    /* Catalan numbers are generated on the fly:
       C(0) = 1
       C(k+1) = C(k) * (2*(2*k+1)) / (k+2)      */
    seq_t catalan_cur = (seq_t)1;  /* C(0) */

    /* binomial(n-i-1, i-1) for i=1 is 1 */
    seq_t binom = (seq_t)1;

    for (idx_t i = 1; i <= half; ++i) {
        /* Update Catalan to C(i) */
        catalan_cur = catalan_cur *
                      (seq_t)(4 * (i - 1) + 2) / (idx_t)(i + 1);

        /* accumulate term: 3^(n-2i) * Catalan(i) * binom */
        seq_t term = pow3_cur;
        term *= catalan_cur;
        term *= binom;
        sum += term;

        if (i < half) {
            /* next 3^(n-2(i+1)) = previous / 9 */
            pow3_cur /= 9;

            /* Update binomial for next i+1:
               B_{i+1} = B_i * (n-2(i+1)+1)*(n-2(i+1)+2) /
                         ((n-(i+1))*(i))                       */
            seq_t num1 = (seq_t)(n - 2 * (i + 1) + 1);
            seq_t num2 = (seq_t)(n - 2 * (i + 1) + 2);
            seq_t den1 = (seq_t)(n - (i + 1));
            seq_t den2 = (seq_t)i;

            binom = binom * num1 * num2 / (den1 * den2);
        }
    }

    return sum;
}
