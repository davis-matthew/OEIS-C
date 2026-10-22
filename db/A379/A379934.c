/*
 * A379934  E.g.f. A(x) satisfies A(x) = 1/( exp(-x*A(x)) - x )^2.
 * https://oeis.org/A379934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A379934
   Formula: a(n) = 2 * n! * Σ_{k=0..n} (n+k+2)^(k-1) *
                 binomial(n+k+2, n-k) / k!   (k=0 term is n! * (n+1)) */
#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Pre‑compute n! once. */
    const seq_t nFact = factorial(n);

    /* k = 0 contributes n! * (n + 1). */
    seq_t sum = nFact * (seq_t)(n + 1);

    /* Sum the remaining terms k = 1 .. n. */
    for (idx_t k = 1; k <= n; ++k) {
        /* Base for the power. */
        const seq_t base = (seq_t)(n + k + 2);

        /* 2 * n! * binomial(n+k+2, n-k). */
        seq_t term = (seq_t)2 * nFact;
        term *= binomial(n + k + 2, n - k);

        /* Divide by k! */
        term /= factorial(k);

        /* Multiply by (n+k+2)^(k-1). */
        term *= big_pow(base, (seq_t)(k - 1));

        /* Accumulate. */
        sum += term;
    }

    return sum;
}
