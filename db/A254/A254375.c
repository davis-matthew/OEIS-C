/*
 * A254375  Digital roots of centered heptagonal numbers (A069099).
 * https://oeis.org/A254375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = digital root of (7*n^2 - 7*n + 2)/2, which is periodic
 *            with period 9
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t tbl[9] = {1, 8, 4, 7, 8, 7, 4, 8, 1};
    idx_t r = (n - 1) % 9;          /* 0 .. 8 for n = 1..∞ */
    return tbl[r];
}
