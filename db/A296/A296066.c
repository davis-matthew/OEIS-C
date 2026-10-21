/*
 * A296066  a(n) = A296065(n)/n.
 * https://oeis.org/A296066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 1 if n>=3 odd; a(n) = 0 if n>=4 even; a(1)=0; a(2)=1
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 1;
    return (n & 1) ? 1 : 0;
}
