/*
 * A341996  a(n) = 1 if there is at least one such prime p that p^p divides
 *          the arithmetic derivative of n, A003415(n); a(0) = a(1) = 0 by
 *          convention.
 * https://oeis.org/A341996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if ∃ prime p with p^p | A003415(n) (arithmetic
 *            derivative of n), else 0. For n=0,1 the function returns 0 by
 *            convention
 * Range:     u8 n <= 9555, u16 n <= 9555, u32 n <= 9555, u64 n <= 9555,
 *            u128 n <= 9555, big any n
 * Helpers:   factor_and_check
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_and_check.h"

seq_t a(idx_t n)
{
    /* Special cases */
    if (n <= 1)
        return 0;

    /* Factor n and compute its arithmetic derivative */
    seq_t derivative = 0;
    idx_t nn = n;
    idx_t p = 2;
    while (p * p <= nn) {
        if (nn % p == 0) {
            int exp = 0;
            while (nn % p == 0) {
                nn /= p;
                exp++;
            }
            /* term = exp * (n / p) */
            idx_t term = (n / p) * exp;
            derivative += (seq_t)term;
        }
        p++;
    }
    if (nn > 1) { /* remaining prime factor */
        /* term = 1 * (n / nn) */
        idx_t term = n / nn;   /* nn is a prime > 1 */
        derivative += (seq_t)term;
    }

    /* Check whether any prime p satisfies p^p | derivative */
    return factor_and_check(derivative) ? 1 : 0;
}
