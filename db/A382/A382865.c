/*
 * A382865  Bitwise XOR of all integers between n and 2n (endpoints included).
 * https://oeis.org/A382865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = XOR_{k=n}^{2n} k
 * Range:     u8 n <= 127, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 82 terms through n = 9867 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t xor_upto(seq_t m) {
    seq_t mod = m & (seq_t)3;
    if (mod == (seq_t)0) return m;                 /* m % 4 == 0 -> m          */
    if (mod == (seq_t)1) return (seq_t)1;          /* m % 4 == 1 -> 1          */
    if (mod == (seq_t)2) return m + (seq_t)1;      /* m % 4 == 2 -> m + 1      */
    return (seq_t)0;                               /* m % 4 == 3 -> 0          */
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    seq_t lo = (seq_t)n;            /* n                    */
    seq_t hi = lo << 1;             /* 2*n                   */
    seq_t xr_hi = xor_upto(hi);     /* XOR 0..2n              */
    seq_t xr_lo1 = xor_upto(lo - (seq_t)1); /* XOR 0..(n-1)  */
    return xr_hi ^ xr_lo1;          /* XOR n..2n = XOR 0..2n ^ XOR 0..(n-1) */
}
