/*
 * A361637  Constant term in the expansion of (1 + x + y + z + 1/(x*y*z))^n.
 * https://oeis.org/A361637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0}^{⌊n/4⌋} 1/(k!^4 * (n-4k)!)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   binomial_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      Also equals the constant term of (1 + x + y + z + 1/(xyz))^n. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial_m.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n <= 3) return 1;

    seq_t sum  = 0;       /* accumulator for the sum */
    seq_t mult = 1;       /* (4k)! / (k!^4) for current k */

    idx_t maxk = n / 4;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* Contribution for current k */
        idx_t m = 4 * k;                    /* the binomial upper index */
        seq_t binom = binomial_m(n, m);     /* C(n,4k) */
        sum += binom * mult;

        if (k == maxk) break;               /* finished */

        /* Update mult for next k:
           mult(k+1) = mult(k) * (4(k+1))(4(k+1)-1)(4(k+1)-2)(4(k+1)-3) / (k+1)^4
        */
        idx_t kp = k + 1;
        seq_t prod  = (seq_t)(4 * kp) *
                      (seq_t)(4 * kp - 1) *
                      (seq_t)(4 * kp - 2) *
                      (seq_t)(4 * kp - 3);
        seq_t denom = (seq_t)kp * kp * kp * kp;
        mult = mult * prod / denom;
    }

    return sum;
}
