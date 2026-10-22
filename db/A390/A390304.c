/*
 * A390304  E.g.f. A(x) satisfies A(x) = exp( x/(1-x^2)^3 * A(x)^2 ).
 * https://oeis.org/A390304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (2*(n-2k)+1)^(n-2k-1) * *
 *            binomial(3*n-5*k-1, k) / (n-2k)! * a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#include "../utils/factorial.h"   /* seq_t factorial(idx_t n) */
#include "../utils/big_pow.h"     /* seq_t big_pow(seq_t base, seq_t exp) */
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k) */

/* Compute a(n) and memoise factorials up to the maximum n seen. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Static table of factorials: fact[i] = i! for i = 0..N */
    static seq_t *fact = NULL;
    static size_t fact_cap = 0;
    size_t needed = (size_t)(n + 1);      /* require fact[n] */

    if (needed > fact_cap) {
        size_t new_cap = needed;
        if (fact == NULL) {
            fact = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!fact) exit(1);           /* allocation failure */
            fact[0] = (seq_t)1;
            for (size_t i = 1; i < new_cap; ++i)
                fact[i] = fact[i - 1] * (seq_t)i;
        } else {
            fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
            if (!fact) exit(1);
            for (size_t i = fact_cap; i < new_cap; ++i)
                fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_cap = new_cap;
    }

    seq_t sum = 0;
    idx_t maxk = n / 2;   /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 2 * k;          /* n-2k  */
        if (m == 0)                   /* skip the term with negative exponent */
            continue;

        /* n! / (n-2k)! */
        seq_t term = fact[(size_t)n] / fact[(size_t)m];

        /* (2*m+1)^(m-1) */
        seq_t pow_term = big_pow((seq_t)(2 * m + 1), (seq_t)(m - 1));

        /* binomial(3*n-5*k-1, k) */
        idx_t N = 3 * n - 5 * k - 1;
        seq_t bin_term = binomial(N, k);

        term = term * pow_term * bin_term;
        sum += term;
    }

    return sum;
}
