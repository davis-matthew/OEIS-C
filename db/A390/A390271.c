/*
 * A390271  E.g.f. A(x) satisfies A(x) = exp( x / (1+x^3) * A(x)^2 ).
 * https://oeis.org/A390271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A390271
   Formula: a(n) = n! * Σ_{k=0..⌊n/3⌋} (-1)^k * (2*(n-3k)+1)^(n-3k-1) *
             binomial(n-2k-1, k) / (n-3k)!     (n ≥ 0)
   Initial value: a(0) = 1
*/

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"      /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/binomial.h"    /* seq_t binomial(idx_t a, idx_t b);  */

seq_t a(idx_t n)
{
    /* Special case */
    if (n == 0) return 1;

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t k = 0; k <= n / 3; ++k) {
        idx_t n3k   = n - 3 * k;                 /* n - 3k  */
        if (n3k == 0) continue;                 /* term vanishes for n-3k = 0 */

        idx_t upper = n - 2 * k - 1;             /* upper index of binomial */
        if ((idx_t)upper < k) continue;          /* binomial would be 0 */

        seq_t bin = binomial(upper, k);          /* binomial(n-2k-1, k) */
        if (bin == 0) continue;

        /* Compute the ratio n! / (n-3k)! as a product of integers.
           This avoids factorial overflow or modulo zero in 64‑bit builds. */
        seq_t ratio = 1;
        for (idx_t t = n3k + 1; t <= n; ++t) {
            ratio *= (seq_t)t;
        }

        /* (2*(n-3k)+1)^(n-3k-1) */
        seq_t base = 2 * (seq_t)n3k + 1;
        seq_t exp  = (seq_t)(n3k - 1);
        seq_t power = big_pow(base, exp);

        /* term = ratio * power * binomial, with alternating sign */
        seq_t term = ratio;
        term *= power;
        term *= bin;

        if (k & 1) {
            sum_neg += term;
        } else {
            sum_pos += term;
        }
    }

    /* Final result: difference of the two partial sums */
    return sum_pos >= sum_neg ? sum_pos - sum_neg : 0;
}
