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
 * Helpers:   big_pow, binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;

    idx_t two_n = n + n;

    /* Binomial coefficient C(2n,n) */
    seq_t comb = binom(two_n, n);

    /* n! and (n-1)! */
    seq_t fact_n          = factorial(n);
    seq_t fact_n_minus1   = factorial(n - 1u);

    /* (2n)^(n-1) */
    seq_t power = big_pow((seq_t)two_n, (seq_t)(n - 1u));

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
