/*
 * A348064  Coefficient of x^3 in expansion of n!* Sum_{k=0..n} binomial(x,k).
 * https://oeis.org/A348064
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * Stirling1(k,3) * (n-k)!
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (equivalently n! / k! * Stirling1(k,3) summed over k) */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* factorials n! stored in fact array */
    static seq_t *fact = NULL;
    static size_t fact_cap = 0;
    static idx_t  fact_size = 0;   /* highest n computed */

    if (fact == NULL) {
        fact_cap = 4;
        fact = (seq_t *)malloc(fact_cap * sizeof(seq_t));
        fact[0] = (seq_t)1;
        fact_size = 0;
    }
    if (fact_size < n) {
        if (fact_cap <= (size_t)n) {
            size_t new_cap = fact_cap ? fact_cap << 1 : 4;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
            if (!tmp) return 0;            /* allocation failure */
            fact = tmp;
            fact_cap = new_cap;
        }
        for (idx_t i = fact_size + 1; i <= n; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        fact_size = n;
    }

    /* Stirling1(k,3) values stored in s3 array */
    static seq_t *s3 = NULL;
    static size_t s3_cap = 0;
    static idx_t  s3_size = 0;   /* highest k computed */

    /* state variables for recurrence of Stirling numbers */
    static seq_t prev_s1_1 = 1;   /* s(1,1)   */
    static seq_t prev_s1_2 = 0;   /* s(1,2)   */
    static seq_t prev_s1_3 = 0;   /* s(1,3)   */

    if (s3 == NULL) {
        s3_cap = 4;
        s3 = (seq_t *)malloc(s3_cap * sizeof(seq_t));
        if (!s3) return 0;              /* allocation failure */
        s3[0] = (seq_t)0;
        s3[1] = (seq_t)0;
        s3[2] = (seq_t)0;
        s3_size = 2;
        /* initialise recurrence state to values after computing k=2 */
        prev_s1_1 = (seq_t)(-1);   /* s(2,1) */
        prev_s1_2 = (seq_t)( 1);   /* s(2,2) */
        prev_s1_3 = (seq_t)( 0);   /* s(2,3) */
    }
    if (s3_size < n) {
        if (s3_cap <= (size_t)n) {
            size_t new_cap = s3_cap ? s3_cap << 1 : 4;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(s3, new_cap * sizeof(seq_t));
            if (!tmp) return 0;            /* allocation failure */
            s3 = tmp;
            s3_cap = new_cap;
        }
        for (idx_t i = s3_size + 1; i <= n; ++i) {
            seq_t new_s1_1 = -(seq_t)(i - 1) * prev_s1_1;
            seq_t new_s1_2 = prev_s1_1 - (seq_t)(i - 1) * prev_s1_2;
            seq_t new_s1_3 = prev_s1_2 - (seq_t)(i - 1) * prev_s1_3;

            s3[i] = new_s1_3;    /* store Stirling1(i,3) */

            prev_s1_1 = new_s1_1;
            prev_s1_2 = new_s1_2;
            prev_s1_3 = new_s1_3;
        }
        s3_size = n;
    }

    /* compute the sum */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = fact[n] / fact[k];   /* n! / k! */
        term *= s3[k];                    /* multiply by Stirling1(k,3) */
        sum += term;
    }
    return sum;
}
