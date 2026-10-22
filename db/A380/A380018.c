/*
 * A380018  Expansion of e.g.f. 1/(exp(-4*x) - 4*x)^(1/4).
 * https://oeis.org/A380018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (-4)^k * (4*k+1)^(n-k) *
 *            binomial(-1/4,k)/(n-k)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the simplified form:
      a(n) = sum_{k=0..n} binom(n,k) * (4k+1)^(n-k) * P_k ,
      where P_k = prod_{j=1..k} (4j-3),  P_0 = 1.
   This version is valid for any unsigned seq_t (uint8_t … uint64_t,
   unsigned __int128, or arbitrary‑precision integer class). */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;          /* cumulative sum of all terms */
    seq_t binom = 1;          /* binom(n,0) = 1 */
    seq_t P     = 1;          /* product (4j-3), P_0 = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        /* (4k+1)^(n-k) */
        seq_t base = (seq_t)(4 * k + 1);
        seq_t exp  = (seq_t)(n - k);
        seq_t pow  = big_pow(base, exp);

        /* term = binom(n,k) * pow * P_k */
        seq_t term = binom * pow;
        term *= P;
        sum += term;

        /* update binom for next k: binom(n,k+1) = binom(n,k) * (n-k)/(k+1) */
        if (k < n) {
            binom = binom * (n - k) / (k + 1);

            /* update P for next k: P_{k+1} = P_k * (4(k+1)-3) */
            P = P * (seq_t)(4 * (k + 1) - 3);
        }
    }
    return sum;
}
