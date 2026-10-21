/*
 * A277360  Number of self-avoiding planar walks starting at (0,0), ending at
 *          (n,n), remaining in the first quadrant and using steps (0,1),
 *          (1,0), (1,1), (-1,1), and (1,-1).
 * https://oeis.org/A277360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = (16*n^2-4*n-1)*a(n-1) - n*(4*n-6)*a(n-2), a(0)=1, a(1)=9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)9;   /* a(1) */
    seq_t ai = (seq_t)0;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t nn = (seq_t)i;
        /* coefficient for a(n-1): 16*n^2 - 4*n - 1 */
        seq_t coef1 = (seq_t)(16 * nn * nn) - (seq_t)(4 * nn) - (seq_t)1;
        /* coefficient for a(n-2): n*(4*n-6) */
        seq_t coef2 = nn * ((seq_t)(4 * nn) - (seq_t)6);
        ai = coef1 * a1 - coef2 * a0;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
