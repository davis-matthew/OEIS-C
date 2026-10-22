/*
 * A367790  E.g.f. satisfies A(x) = exp( x/(1-x)^4 * A(x) ).
 * https://oeis.org/A367790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..n} (k+1)^(k-1) * binomial(n+3k-1, n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer formula: a(n) = Σ_{k=1..n} (n!/k!) * (k+1)^(k-1) * binomial(n+3k-1, n-k) */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The first term (n = 0) is 1. */
    if (n == 0U)
        return (seq_t)1;

    /* Static factorial cache up to the largest n seen so far. */
    static idx_t fact_n = 0U;          /* highest n for which factorials are known */
    static seq_t *fact  = NULL;        /* factorial array: fact[i] = i!   */
    static size_t fact_cap = 0U;       /* current capacity of the array       */

    /* Grow or initialise the factorial array when needed. */
    if (n > fact_n) {
        size_t new_cap = (size_t)(n + 1U);
        if (!fact) {
            fact = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!fact)
                return (seq_t)0;   /* out of memory – caller may interpret 0 as failure */

            fact[0] = (seq_t)1;
            for (idx_t i = 1U; i <= n; ++i)
                fact[i] = fact[i - 1] * (seq_t)i;
        } else {
            if (new_cap > fact_cap) {
                seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
                if (!tmp)
                    return (seq_t)0;
                fact = tmp;
                for (idx_t i = fact_cap; i <= n; ++i)
                    fact[i] = fact[i - 1] * (seq_t)i;
            } else { /* extend already existing array */
                for (idx_t i = fact_n + 1U; i <= n; ++i)
                    fact[i] = fact[i - 1] * (seq_t)i;
            }
        }
        fact_n   = n;
        fact_cap = new_cap;
    }

    seq_t sum = (seq_t)0;          /* accumulator for the sum */
    seq_t mult = fact[n];          /* will hold n! / k! after each division */

    /* k starts at 1; for k = 0 the binomial term is zero for n > 0. */
    for (idx_t k = 1U; k <= n; ++k) {
        mult /= k; /* now mult = n! / k! */

        /* (k+1)^(k-1)  (k+1) and k-1 are small enough for the generic big_pow). */
        seq_t pow_term = big_pow((seq_t)(k + 1U), (seq_t)(k - 1U));

        /* binomial(n+3k-1, n-k)   (top >= bottom for k ≤ n). */
        seq_t bin = binomial(n + 3U * k - 1U, n - k);

        /* term = (n!/k!) * (k+1)^(k-1) * binomial */
        seq_t term = mult * pow_term;
        term = term * bin;

        sum += term;
    }

    return sum;
}
