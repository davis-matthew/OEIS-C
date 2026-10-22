/*
 * A372164  E.g.f. A(x) satisfies A(x) = exp( 2 * x * A(x)^(5/2) * (1 + x *
 *          A(x)) ).
 * https://oeis.org/A372164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A372164: a(n) = 2 * n! * Σ_{k=⌈n/2⌉}^{n} (3k + 2n + 2)^(k‑1) /
   ((n‑k)! * (2k‑n)!)  */
#include <stddef.h>
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;              /* first term */

    const seq_t r = (seq_t)2;
    const seq_t nfact = factorial(n);           /* n! */

    /* binomial(k, n-k) is non‑zero only for k ≥ ceil(n/2) */
    const idx_t start = (n + 1) / 2;            /* ceil(n/2) */

    seq_t sum = 0;

    for (idx_t k = start; k <= n; ++k)
    {
        /* base for powering: 3k + 2n + 2 */
        const seq_t base = (seq_t)3 * (seq_t)k
                        + (seq_t)2 * (seq_t)n
                        + (seq_t)2;

        const seq_t power = big_pow(base, (seq_t)(k - 1));

        /* denominator: (n-k)! * (2k-n)! */
        const seq_t denom = factorial(n - k) *
                            factorial((idx_t)(2 * k - n));

        const seq_t coeff = nfact / denom;      /* exact division */

        const seq_t term = coeff * power;
        sum += term;
    }

    return r * sum;
}
