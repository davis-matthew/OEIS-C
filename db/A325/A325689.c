/*
 * A325689  Number of length-3 compositions of n such that no part is the sum
 *          of the other two.
 * https://oeis.org/A325689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -(5 + 3*(-1)^n - 2*n) * (n-2) / 4 (for n>0)
 * Range:     u8 n <= 24, u16 n <= 364, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 0, a(0) = 0 */

#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    /* Use a signed 128‑bit integer for intermediate calculations
       to avoid overflow for reasonably large n. */
    __int128 nn   = n;
    __int128 parity = (n & 1) ? -1 : 1;                   /* (-1)^n   */
    __int128 t = 5 + 3 * parity - 2 * nn;                 /* 5 + 3*(-1)^n - 2*n */
    __int128 num = -t * (nn - 2);                        /* -t*(n-2)   */
    __int128 res = num / 4;                              /* division by 4 (exact) */

    return (seq_t)res;
}
