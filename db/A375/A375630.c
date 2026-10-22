/*
 * A375630  Expansion of e.g.f. exp(2*x) / (1 - x^3 * exp(x)).
 * https://oeis.org/A375630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * ∑_{k=0..⌊n/3⌋} (k+2)^(n-3k)/(n-3k)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* local integer‑exponentiation for seq_t, no external dependencies */
static seq_t power_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t cur     = base;
    while (exp) {
        if (exp & (idx_t)1)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;          /* highest k with n-3k ≥ 0 */
    seq_t sum = (seq_t)0;
    seq_t ff  = (seq_t)1;        /* ff = n!/(n-3k)! for current k */

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 3 * k;                     /* exponent (n-3k) */
        seq_t pow = power_seq((seq_t)(k + 2), m); /* (k+2)^(m) */

        sum += ff * pow;                         /* add term */

        /* update ff for next k+1 : multiply by (m)*(m-1)*(m-2) */
        if (k < maxk) {
            ff *= (seq_t)m;
            ff *= (seq_t)(m - 1);
            ff *= (seq_t)(m - 2);
        }
    }
    return sum;
}
