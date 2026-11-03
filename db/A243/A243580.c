/*
 * A243580  Integers of the form 8k + 7 that can be written as a sum of four
 *          distinct squares of the form m, m + 1, m + 3, m + 5, where m == 2
 *          (mod 4).
 * https://oeis.org/A243580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64*n^2 + 8*n + 15
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 8191, u64 n <= 20737,
 *            u128 n <= 20737, big any n
 * Validated: standard, pass, 64 terms through n = 8808 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    return sn * sn * 64 + sn * 8 + 15;
}
