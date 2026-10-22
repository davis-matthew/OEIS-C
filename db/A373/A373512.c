/*
 * A373512  Expansion of 1/(1 - x/(1 - 16*x^8)^(1/8)).
 * https://oeis.org/A373512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, for 0 <= n < 8, otherwise a(n) = Σ_{k=0..⌊n/8⌋} 16^k *
 *            binomial((n-8)/8, k) (rational binomial). This is computed by
 *            the recurrence: b_0 = 1 b_k = (2 * ( (n-8) - 8*(k-1) ) / k) *
 *            b_{k-1} a(n) = Σ_{k=0..⌊n/8⌋} b_k All operations are performed
 *            using seq_t, the arbitrary‑precision integer...
 * Range:     u8 n <= 20, u16 n <= 35, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n)
{
    idx_t m = n / 8;           /* floor(n/8)          */
    if (m == 0)                /* n < 8               */
        return 1;

    seq_t sum  = 1;            /* b_0                 */
    seq_t term = 1;            /* b_{k-1}             */
    seq_t R    = (seq_t)n - 8; /* R = n - 8           */

    for (idx_t k = 1; k <= m; ++k) {
        /* Multiply by 2 * (R - 8*(k-1))  */
        term = term * ((seq_t)2 * (R - ((seq_t)8 * (k - 1))));
        /* Exact division by k            */
        term = term / (seq_t)k;
        sum += term;
    }
    return sum;
}
