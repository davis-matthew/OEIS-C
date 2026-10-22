/*
 * A387950  E.g.f. A(x) satisfies A(x) = exp( x^3*A(x)^3 / (1 - x*A(x))^2 ).
 * https://oeis.org/A387950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (n+1)^(k-1) *
 *            binomial(n-k-1,n-3*k)/k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   binomial, factorial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Special case: a(0) = 1 */

#include <stdbool.h>
#include "../utils/factorial.h"   /* seq_t factorial(idx_t n); */
#include "../utils/binomial.h"    /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/pow_seq.h"     /* seq_t pow_seq(seq_t base, int exp); */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Maximum value of k in the sum */
    idx_t max_k = n / 3;

    /* Precompute factorials up to max_k */
    static seq_t fact_cache[3335];     /* 10000/3 ≈ 3333, +1 for safety */
    fact_cache[0] = (seq_t)1;
    for (idx_t i = 1; i <= max_k; ++i)
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;

    /* factorial(n) */
    seq_t fact_n = factorial(n);

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= max_k; ++k)
    {
        /* term = n! / k! * (n+1)^(k-1) * binomial(n-k-1,n-3*k) */
        seq_t term = fact_n / fact_cache[k];
        term = term * pow_seq((seq_t)(n + 1), (int)(k - 1));
        term = term * binomial(n - k - 1, n - 3 * k);
        sum = sum + term;
    }
    return sum;
}
