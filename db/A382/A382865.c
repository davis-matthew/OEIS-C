/*
 * A382865  Bitwise XOR of all integers between n and 2n (endpoints included).
 * https://oeis.org/A382865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = XOR_{k=n}^{2n} k
 * Range:     u8 n <= 127, u16 n <= 9867, u32 n <= 9867, u64 n <= 9867,
 *            u128 n <= 9867, big any n
 * Helpers:   xor_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/xor_upto.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    seq_t lo = (seq_t)n;            /* n                    */
    seq_t hi = lo << 1;             /* 2*n                   */
    seq_t xr_hi = xor_upto(hi);     /* XOR 0..2n              */
    seq_t xr_lo1 = xor_upto(lo - (seq_t)1); /* XOR 0..(n-1)  */
    return xr_hi ^ xr_lo1;          /* XOR n..2n = XOR 0..2n ^ XOR 0..(n-1) */
}
