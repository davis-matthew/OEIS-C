/*
 * A350327  Maximum domination number of connected graphs with n vertices and
 *          minimum degree 2.
 * https://oeis.org/A350327
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n)/5), except a(4)=2, a(7)=3
 * Range:     u8 n <= 541, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return 0;          /* undefined below the offset */
    if (n == 4) return (seq_t)2;   /* exceptional case */
    if (n == 7) return (seq_t)3;   /* exceptional case */
    return ((seq_t)n * 2) / 5;     /* floor(2*n/5) for all other n ≥ 3 */
}
