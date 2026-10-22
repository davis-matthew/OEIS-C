/*
 * A376492  E.g.f. satisfies A(x) = exp(x^2 * (1 + x) * A(x)^2).
 * https://oeis.org/A376492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/2⌋} (2k+1)^{k‑1} * binomial(k, n‑2k)/k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided binomial() and pow_uint() from the utils bundle. */

#include <stdlib.h>
#include "../utils/binomial.h"   /* uint64_t binomial(idx_t n, idx_t k)   */
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* Trivial case */
    if (n == 0)
        return (seq_t)1;

    /* ------- factor cache (n! up to current n) --------- */
    static seq_t *fact = NULL;
    static idx_t fact_cap = 0;      /* size of allocated array */

    if (n + 1 > fact_cap) {
        idx_t old_cap = fact_cap;
        fact_cap = n + 1;
        fact = (fact == NULL) ? (seq_t*)malloc(fact_cap * sizeof(seq_t))
                               : (seq_t*)realloc(fact, fact_cap * sizeof(seq_t));
        if (fact == NULL)          /* allocation failed – return 0 as sentinel */
            return (seq_t)0;

        /* initialise the factorial array from the first missing index */
        if (old_cap == 0) {
            fact[0] = (seq_t)1;
            old_cap = 1;
        }
        for (idx_t i = old_cap; i <= n; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
    }

    seq_t result = (seq_t)0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t r = n - 2 * k;                 /* n – 2k */
        if (r > k)                            /* binomial(k,n-2k) = 0 for r>k */
            continue;

        seq_t c   = binomial(k, r);          /* binomial(k, n-2k) */
        seq_t pw;
        if (k == 0)                           /* (2*0+1)^(−1) = 1 by convention */
            pw = (seq_t)1;
        else
            pw = pow_uint((uint64_t)(2 * k + 1), k - 1);

        /* term = (n! / k!) * c * pw */
        seq_t term = fact[n];
        term /= fact[k];
        term *= c;
        term *= pw;

        result += term;
    }

    return result;
}
