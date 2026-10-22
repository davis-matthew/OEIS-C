/*
 * A390014  E.g.f. A(x) satisfies A(x) = exp( x * (1-x^3) * A(x) ).
 * https://oeis.org/A390014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/4⌋} (-1)^k * (n-3k+1)^(n-3k-1) * *
 *            binomial(n-3k,k) / (n-3k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   binomial, factorial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses: factorial(idx_t n), binomial(idx_t n, idx_t k), seq_pow(seq_t base, idx_t exp) */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Trivial case n = 0 */
    if (n == 0)
        return 1;

    /* n! */
    seq_t nfact = factorial(n);

    seq_t result = 0;
    idx_t max_k = n / 4;

    for (idx_t k = 0; k <= max_k; ++k)
    {
        idx_t m = n - 3 * k;                /* m = n - 3k */
        idx_t exp = m - 1;                  /* exponent for the power term */

        /* (n-3k+1)^(n-3k-1) */
        seq_t pow_val = seq_pow((seq_t)(m + 1), exp);

        /* binomial(n-3k, k) */
        seq_t bin = binomial(m, k);

        /* (n!)/(m!) = product of last 3k terms of n! */
        seq_t fm = factorial(m);
        seq_t ratio = nfact / fm;

        /* term = ratio * bin * pow_val */
        seq_t term = ratio * bin;
        term = term * pow_val;

        /* Apply sign (-1)^k */
        if (k & 1)
            result -= term;
        else
            result += term;
    }

    return result;
}
