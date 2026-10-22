/*
 * A389175  Period of all n-th order linear recurrence integers sequence with
 *          constant integer coefficients mod 9.
 * https://oeis.org/A389175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = 27 · LCM{ 3^k – 1 | 1 ≤ k ≤ n } for n ≥ 4 Special cases:
 *            a(0)=1, a(1)=6, a(2)=72, a(3)=936 (values taken from the OEIS
 *            entry for A389175)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Computes the n-th term of sequence A389175.
   idx_t and seq_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* handle small indices explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)72;
    if (n == 3) return (seq_t)936;

    /* LCM of (3^k - 1) for k = 1 .. n */
    seq_t lcm_val = (seq_t)1;
    seq_t pow3    = (seq_t)1;          /* pow3 = 3^k */

    for (idx_t k = 1; k <= n; ++k) {
        pow3 *= (seq_t)3;              /* pow3 = 3^k      */
        seq_t val = pow3 - (seq_t)1;   /* val   = 3^k - 1 */
        seq_t g   = gcd(lcm_val, val);
        lcm_val = (lcm_val / g) * val;
    }

    /* multiply by 27 according to the formula */
    lcm_val *= (seq_t)27;
    return lcm_val;
}
