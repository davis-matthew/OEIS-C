/*
 * A247161  Dynamic Betting Game D(n,4,2).
 * https://oeis.org/A247161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(12*n/11) + floor((4*n+1)/11)
 * Range:     u8 n <= 175, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (n ≥ 1; returns 0 for n==0 for safety) */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t sn = (seq_t)n;
    seq_t t1 = (sn * (seq_t)12) / (seq_t)11;
    seq_t t2 = ((sn * (seq_t)4) + (seq_t)1) / (seq_t)11;
    return t1 + t2;
}
