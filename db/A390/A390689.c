/*
 * A390689  The sum of the divisors of n that are sums of 2 squares (A001481).
 * https://oeis.org/A390689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n, d is sum of two squares} d Implemented as a
 *            multiplicative function using prime‑power formulas
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_even_powers, sum_powers
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sum_even_powers.h"
#include "../utils/sum_powers.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined case, safe guard */
    if (n == 1) return (seq_t)1;   /* a(1) = 1 */

    idx_t temp = n;
    seq_t ans  = 1;

    /* factor n and compute contribution for each prime factor */
    for (idx_t p = 2; p <= temp / p; p += (p == 2 ? 1 : 2)) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            if (p % 4 == 3) {
                /* p ≡ 3 (mod 4) : only even exponents allowed */
                idx_t k = e / 2;               /* floor(e/2) */
                seq_t contrib = sum_even_powers(p, k);
                ans *= contrib;
            } else {
                /* p = 2 or p ≡ 1 (mod 4): all exponents allowed */
                seq_t contrib = sum_powers(p, e);
                ans *= contrib;
            }
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        idx_t lp = temp;   /* exponent e = 1 */
        if (lp % 4 == 3) {
            /* contribution is 1, no change to ans */
        } else {
            ans *= ((seq_t)1 + (seq_t)lp);
        }
    }

    return ans;
}
