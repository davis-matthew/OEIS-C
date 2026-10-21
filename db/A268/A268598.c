/*
 * A268598  Expansion of x^5*(4 - 5*x)/(1 - 2*x)^4.
 * https://oeis.org/A268598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 25, u64 n <= 53, u128 n <= 114,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268598 – expansion of x^5*(4-5*x)/(1-2*x)^4
   Formula: a(n) = 0          (n ≤ 4)
            a(5) = 4, a(6) = 27
            a(n) = 2^(n-7)*(n-4)*(n-3)*(n+3)  (n ≥ 7)  */

/* Uses the utility function pow2 from the build’s utils folder. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 4) return 0;
    if (n == 5) return 4;
    if (n == 6) return 27;

    seq_t p = pow2(n - 7);                /* 2^(n-7) */
    return p * (seq_t)(n - 4) * (seq_t)(n - 3) * (seq_t)(n + 3);
}
