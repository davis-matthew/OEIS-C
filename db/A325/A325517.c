/*
 * A325517  a(n) = n*((2*n + 1)*(2*n^2 + 2*n + 3) - 3*(-1)^n)/24.
 * https://oeis.org/A325517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*((2*n + 1)*(2*n^2 + 2*n + 3) - 3*(-1)^n)/24
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 400, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Use 128‑bit arithmetic for intermediate values to avoid overflow
       when seq_t is 64‑bit.  The result can always be cast back to seq_t
       because the final value fits in the same type. */
    unsigned __int128 un = n;
    unsigned __int128 term1 = 2 * un + 1;                      // 2n + 1
    unsigned __int128 term2 = 2 * un * un + 2 * un + 3;        // 2n^2 + 2n + 3
    unsigned __int128 s   = term1 * term2;                     // (2n+1)*(2n^2+2n+3)
    if ((n & 1) == 0) {    /* n even: (-1)^n = 1, so subtract 3 */
        s -= 3;
    } else {               /* n odd: (-1)^n = -1, so add 3 */
        s += 3;
    }
    unsigned __int128 result128 = un * s / 24;                 // n*s/24
    return (seq_t)result128;
}
