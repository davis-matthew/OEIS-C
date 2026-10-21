/*
 * A293042  Number of even permutations p of {1,...,n} such that p(i) is not i
 *          or i+1.
 * https://oeis.org/A293042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A000271(n) + (-1)^(n-1) * floor((n-1)/2))/2
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000271(n) is the number of permutations of {1,…,n} avoiding p(i)=i and p(i)=i+1. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* persistent factorial cache */
    static idx_t fact_cap = 0;
    static seq_t *fact = NULL;

    /* initialize factorial cache if necessary */
    if (fact == NULL) {
        fact_cap = 1;
        fact = (seq_t *)malloc(sizeof(seq_t));
        fact[0] = 1;
    }

    /* extend factorial table up to n if required */
    if (n >= fact_cap) {
        fact = (seq_t *)realloc(fact, (n + 1) * sizeof(seq_t));
        for (idx_t i = fact_cap; i <= n; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_cap = n + 1;
    }

    /* Special case n = 0 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute T(n) = sum_{k=0}^{n} (-1)^k * C(2n-k, k) * (n-k)!   */
    seq_t pos = 0;   /* sum of even k terms */
    seq_t neg = 0;   /* sum of odd  k terms */
    seq_t r   = 1;   /* r_0 = C(2n, 0) = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = r * fact[n - k];
        if ((k & 1) == 0) {
            pos += term;
        } else {
            neg += term;
        }

        if (k == n) break;

        /* Compute r_{k+1} from r_k:
           r_{k+1} = r_k * (2n-2k)*(2n-2k-1) / ((k+1)*(2n-k))  */
        seq_t num1 = (seq_t)(2 * (seq_t)n - 2 * (seq_t)k);
        seq_t num2 = (seq_t)(2 * (seq_t)n - 2 * (seq_t)k - 1);
        seq_t den1 = (seq_t)(k + 1);
        seq_t den2 = (seq_t)(2 * (seq_t)n - (seq_t)k);

        r = (r * num1) / den1;
        r = (r * num2) / den2;
    }

    seq_t T;
    if (pos >= neg) {
        T = pos - neg;
    } else {
        /* Theoretically should not happen because T(n) > 0 */
        T = neg - pos;
    }

    /* Compute the final result a(n) = (T + diff)/2 where
       diff = (-1)^(n-1) * floor((n-1)/2).  */
    idx_t half = (n - 1) / 2;          /* floor((n-1)/2) */
    seq_t a_val;
    if (((n - 1) & 1) == 0) {          /* even exponent => + */
        a_val = (T + (seq_t)half) / 2;
    } else {                          /* odd exponent => - */
        a_val = (T - (seq_t)half) / 2;
    }

    return a_val;
}
