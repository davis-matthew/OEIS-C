/*
 * A389625  a(0) = 1; a(n) = (11*n^2 - 9*n + 4)/2 for n>0.
 * https://oeis.org/A389625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n^2 - 9*n + 4)/2 for n>0, a(0) = 1
 * Range:     u8 n <= 7, u16 n <= 109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    /* Use a wider integer for the intermediate calculation to avoid overflow on small seq_t types */
    unsigned __int128 nn = n;
    unsigned __int128 val = (nn * (nn * 11 - 9) + 4) / 2;
    return (seq_t)val;
}
