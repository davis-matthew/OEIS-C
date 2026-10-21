/*
 * A294456  a(1)=0, a(2)=1; thereafter a(n) = a(floor(n/2)) + a(ceiling(n/2))
 *          + 2.
 * https://oeis.org/A294456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 160, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=1; a(n)=a(floor(n/2))+a(ceil(n/2))+2 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    seq_t left  = a(n >> 1);
    seq_t right = a((n + 1) >> 1);
    return left + right + (seq_t)2;
}
