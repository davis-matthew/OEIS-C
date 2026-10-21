/*
 * A267649  a(0) = a(1) = 2 then a(n) = 4 for n>=2.
 * https://oeis.org/A267649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cons, less
 * Formula:   a(0)=a(1)=2, a(n)=4 for n>=2
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) {
        return (seq_t)2;
    }
    return (seq_t)4;
}
