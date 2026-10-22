/*
 * A363644  Expansion of Sum_{k>0} x^(3*k)/(1 - k*x^k)^3.
 * https://oeis.org/A363644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} (n/d)^(d‑3) * binomial(d-1,2)
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utilities required for the implementation. */
#include <stdint.h>
#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/binom.h"    /* seq_t binom(idx_t a, idx_t b); */

/* Computes term a(n) for n ≥ 1.  External linkage is required. */
seq_t a(idx_t n)
{
    /* Accumulator for the sum. */
    seq_t total = 0;

    /* Enumerate divisor pairs (d, n/d).  Only divisors d ≥ 3 contribute. */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            if (d1 >= 3) {
                seq_t base   = (seq_t)(n / d1);
                seq_t exp    = (seq_t)(d1 - 3);
                seq_t power  = big_pow(base, exp);
                seq_t coeff  = binom(d1 - 1, 2);
                total += power * coeff;
            }

            idx_t d2 = n / d1;
            if (d2 != d1 && d2 >= 3) {
                seq_t base   = (seq_t)(n / d2);
                seq_t exp    = (seq_t)(d2 - 3);
                seq_t power  = big_pow(base, exp);
                seq_t coeff  = binom(d2 - 1, 2);
                total += power * coeff;
            }
        }
    }

    return total;
}
