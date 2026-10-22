/*
 * A376098  Expansion of e.g.f. -LambertW(-2*x / (1 - x))/2.
 * https://oeis.org/A376098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula (OEIS A376098): 
 *   a(n) = n! * Σ_{k=1..n} (2*k)^(k-1) * binomial(n-1,k-1) / k! 
 *   Equivalently, a(n) = Σ_{k=1..n} binomial(n-1,k-1) * (2*k)^(k-1) * (n!/k!).
 *   The sequence starts a(0)=0, a(1)=1, a(2)=6, …
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sum  = 0;  /* accumulator for the sum */
    seq_t prod = 1;  /* product (n! / k!) for current k, starts with k = n */

    /* iterate k from n down to 1 */
    for (idx_t k = n; k > 0; --k)
    {
        /* binomial coefficient binomial(n‑1, k‑1)   */
        seq_t binom = binomial(n - 1, k - 1);

        /* power term (2*k)^(k‑1)   */
        seq_t power = seq_pow((seq_t)(2 * k), k - 1);

        /* term = binom * power * prod   (prod = n! / k!)   */
        seq_t term = binom;
        term *= power;
        term *= prod;

        /* add to running total */
        sum += term;

        /* update prod for next iteration: multiply by k (for k‑1)   */
        prod *= (seq_t)k;
    }

    return sum;
}
