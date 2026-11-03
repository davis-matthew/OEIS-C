/*
 * A246050  Number of endofunctions on [2n] where the smallest cycle length
 *          equals n.
 * https://oeis.org/A246050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2n,n)*(n‑1)!*(2n)^(n‑1) + C(2n,n)/2*((n‑1)!)^2 (for
 *            n=0, a(0)=1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 185 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;

    idx_t two_n = n + n;

    /* Binomial coefficient C(2n,n) */
    seq_t comb = binomial(two_n, n);

    /* n! and (n-1)! */
    seq_t fact_n          = factorial(n);
    seq_t fact_n_minus1   = factorial(n - 1u);

    /* (2n)^(n-1) */
    seq_t power = ipow((seq_t)two_n, (seq_t)(n - 1u));

    /* Term contributed by a single n‑cycle with trees hanging from it */
    seq_t termA = comb;
    termA *= fact_n;
    termA *= power;

    /* Term contributed by two disjoint n‑cycles with no trees */
    seq_t comb_half = comb / (seq_t)2;
    seq_t fact_sq   = fact_n_minus1 * fact_n_minus1;
    seq_t termB = comb_half;
    termB *= fact_sq;

    return termA + termB;
}
