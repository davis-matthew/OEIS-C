/*
 * A290642  a(n) is the maximum diameter for an n-threaded binary program. In
 *          other words, a(n) is the maximal finite distance in the transition
 *          graph of an n-threaded binary program.
 * https://oeis.org/A290642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = 3*n for n >= 5; special cases
 *            a(1)=3,a(2)=7,a(3)=13,a(4)=14
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety guard – the sequence starts at n=1 */
    if (n == 1) return 3;
    if (n == 2) return 7;
    if (n == 3) return 13;
    if (n == 4) return 14;
    return (seq_t)n * 3;
}
