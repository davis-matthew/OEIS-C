/*
 * A386836  a(n) = Sum_{k=0..n} binomial(3*n+2,k) * binomial(3*n-k-1,n-k).
 * https://oeis.org/A386836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*n+2, k) * binomial(3*n-k-1, n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* three_n = 3*n */
    seq_t three_n = (seq_t)n * (seq_t)3;

    /* Compute the initial value of the second binomial:
       binom(3*n-1, n). */
    seq_t cur2 = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        seq_t numerator   = three_n - (seq_t)i;           /* (3n-1) - (i-1) = 3n-i */
        seq_t denominator = (seq_t)i;
        cur2 = cur2 * numerator / denominator;
    }

    /* cur1 starts as binom(3*n+2, 0) = 1 */
    seq_t cur1 = (seq_t)1;

    seq_t sum = (seq_t)0;
    const seq_t base_num1 = three_n + (seq_t)2;   /* 3*n + 2 */

    for (idx_t k = (idx_t)0; k <= n; ++k) {
        /* Add current term */
        sum += cur1 * cur2;

        /* Stop when the last term is processed */
        if (k == n)
            break;

        seq_t k_seq     = (seq_t)k;
        seq_t num1      = base_num1 - k_seq;                    /* 3n+2 - k */
        seq_t den1      = k_seq + (seq_t)1;                     /* k+1 */
        cur1 = cur1 * num1 / den1;                              /* binom update */

        seq_t num2      = (seq_t)n - k_seq;                     /* n-k   */
        seq_t den2      = three_n - k_seq - (seq_t)1;           /* 3n-k-1 */
        cur2 = cur2 * num2 / den2;                              /* binom update */
    }

    return sum;
}
