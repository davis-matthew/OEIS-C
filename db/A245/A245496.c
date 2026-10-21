/*
 * A245496  a(n) = n! * [x^n] (exp(x)+x)^n.
 * https://oeis.org/A245496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} (k^k / k!) * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the equivalent integer form: a(n) = Sum_{k=0..n} binomial(n,k) * k^k * (n! / k!) */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* -----------------------------------------------------------------
     * Lazy factorial cache: fact[i] = i!
     * ----------------------------------------------------------------- */
    static seq_t *fact = NULL;
    static idx_t fact_cap = 0;      /* number of entries currently stored */

    if (fact == NULL) {
        fact = (seq_t *)malloc(sizeof(seq_t));
        if (fact == NULL) return (seq_t)0;   /* allocation failure */
        fact[0] = (seq_t)1;
        fact_cap = 1;
    }

    /* Grow the factorial cache up to n */
    while (n >= fact_cap) {
        idx_t i = fact_cap;
        seq_t *tmp = (seq_t *)realloc(fact, (i + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        fact = tmp;
        fact[i] = fact[i - 1] * ((seq_t)i);
        fact_cap = i + 1;
    }

    /* -----------------------------------------------------------------
     * Compute the sum S = Σ_{k=0..n} binomial(n,k) * k^k * (n! / k!).
     * ----------------------------------------------------------------- */
    seq_t sum  = (seq_t)0;
    seq_t binom = (seq_t)1;          /* binomial(n,0) */
    seq_t fact_k = (seq_t)1;         /* k!  */
    seq_t fac_ratio = fact[n];       /* n! / k!   */

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute k^k, with 0^0 taken as 1 */
        seq_t pow_kk = (k == 0) ? (seq_t)1 : seq_pow((seq_t)k, k);

        /* term = binom * k^k * fac_ratio  (integer arithmetic only) */
        seq_t term = binom * pow_kk;
        term = term * fac_ratio;

        sum = sum + term;

        if (k == n) break;          /* no further updates after last k */

        /* Update binomial coefficient to binom(n,k+1) */
        binom = (binom * ((seq_t)(n - k))) / ((seq_t)(k + 1));

        /* Update k! to (k+1)! and fac_ratio to n!/(k+1)! */
        fact_k = fact_k * ((seq_t)(k + 1));
        fac_ratio = fac_ratio / ((seq_t)(k + 1));
    }

    return sum;
}
