/*
 * A362699  Expansion of e.g.f. 1/(1 + LambertW(-x * exp(x^3))).
 * https://oeis.org/A362699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/3⌋} (n-3k)^(n-2k) / (k! * (n-3k)!)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/binom.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    idx_t kmax = n / 3;
    seq_t sum = 0;
    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t m = n - 3 * k;            /* base in the power */
        idx_t e = n - 2 * k;            /* exponent in the power */

        seq_t power = big_pow((seq_t)m, (seq_t)e);

        /* coefficient = n! / (k! * (n-3k)!) =
           binom(n, k) * binom(n-k, 2k) * (2k)!   */
        seq_t coeff = binom(n, k);
        coeff = coeff * binom(n - k, 2 * k);
        coeff = coeff * factorial(2 * k);

        sum = sum + power * coeff;
    }
    return sum;
}
