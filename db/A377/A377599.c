/*
 * A377599  E.g.f. satisfies A(x) = exp( x * A(x) / (1-x)^2 ) / (1-x).
 * https://oeis.org/A377599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+1)^(k-1) * binomial(n+2k, n-k) / k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* factorial[i] = i! for 0 <= i < fact_size */
    static seq_t *fact = NULL;
    static idx_t fact_size = 0;

    /* pow_arr[k] = (k+1)^(k-1)  for 0 <= k < pow_size */
    static seq_t *pow_arr = NULL;
    static idx_t pow_size = 0;

    /* required sizes */
    idx_t needed_fact = 3 * n + 1;
    idx_t needed_pow  = n + 1;

    /* grow factorial array if needed */
    if (fact_size < needed_fact) {
        fact = (seq_t *)realloc(fact, needed_fact * sizeof(seq_t));
        if (!fact) return (seq_t)0; /* allocation failure */
        if (fact_size == 0) {
            fact[0] = (seq_t)1;
            fact_size = 1;
        }
        for (idx_t i = fact_size; i < needed_fact; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        fact_size = needed_fact;
    }

    /* grow power array if needed */
    if (pow_size < needed_pow) {
        pow_arr = (seq_t *)realloc(pow_arr, needed_pow * sizeof(seq_t));
        if (!pow_arr) return (seq_t)0; /* allocation failure */
        if (pow_size == 0)
            pow_arr[0] = (seq_t)1; /* (0+1)^(0-1) treated as 1 */
        for (idx_t k = pow_size; k < needed_pow; ++k) {
            if (k == 0)
                pow_arr[k] = (seq_t)1;
            else
                pow_arr[k] = seq_pow((seq_t)(k + 1), (idx_t)(k - 1));
        }
        pow_size = needed_pow;
    }

    seq_t sum = (seq_t)0;
    seq_t factorial_n = fact[n];
    seq_t ratio = factorial_n; /* n! / k! for current k */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t b = binomial(n + 2 * k, n - k);
        seq_t term = ratio * b;
        term = term * pow_arr[k];
        sum += term;

        if (k < n)
            ratio = ratio / (k + 1);
    }

    return sum;
}
