/*
 * A380082  Number of Boolean intervals of rank 2 in the weak order of the
 *          Coxeter group of type C_n, for n >= 2.
 * https://oeis.org/A380082
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * binomial(n - 1, 2) * 2^(n - 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised factorials: fact[i] = i! for 0 <= i <= max_fact */
    static seq_t *fact = NULL;
    static size_t fact_cap = 0;
    static size_t fact_len = 0;      /* current computed size */

    /* Ensure factorials are computed up to n */
    if (fact_len == 0) {
        /* initialise with 0! = 1 */
        fact_cap = 16;
        fact = (seq_t *)malloc(fact_cap * sizeof(seq_t));
        if (!fact) { return 0; }     /* memory failure: return 0 */
        fact[0] = (seq_t)1;
        fact_len = 1;
    }

    /* Grow factorial array if needed */
    while (fact_len <= n) {
        if (fact_len == fact_cap) {
            fact_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(fact, fact_cap * sizeof(seq_t));
            if (!tmp) { return 0; }  /* memory failure: return 0 */
            fact = tmp;
        }
        idx_t i = fact_len;
        seq_t v = fact[i - 1];
        v *= (seq_t)i;               /* i! = (i-1)! * i */
        fact[i] = v;
        fact_len++;
    }

    /* factorial(n) */
    seq_t fact_n = fact[n];

    /* binomial(n-1,2) = (n-1)*(n-2)/2 */
    seq_t binom = ((seq_t)(n - 1) * (n - 2)) / (seq_t)2;

    /* 2^(n-2) by repeated doubling (shift left is also fine) */
    seq_t pow2 = (seq_t)1;
    if (n >= 2) {
        for (idx_t i = 0; i < n - 2; ++i) {
            pow2 *= (seq_t)2;
        }
    }

    /* Result */
    seq_t result = fact_n * binom;
    result *= pow2;
    return result;
}
