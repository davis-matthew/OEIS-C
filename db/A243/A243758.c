/*
 * A243758  a(n) = Product_{i=1..n} A234959(i).
 * https://oeis.org/A243758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^( Σ_{k≥1} ⌊n / 6^k⌋ ), i.e. the product of 6^v6(i) over
 *            i=1..n
 * Range:     u8 n <= 23, u16 n <= 35, u32 n <= 71, u64 n <= 131,
 *            u128 n <= 251, big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* sum floor(n / 6^k) over all k >= 1 */
    idx_t power   = 6;
    idx_t exponent = 0;

    while (power <= n) {
        exponent += n / power;

        /* prevent overflow of power on next multiplication by 6 */
        if (power > n / 6)
            break;
        power *= 6;
    }

    /* return 6^exponent */
    return pow_int((seq_t)6, exponent);
}
