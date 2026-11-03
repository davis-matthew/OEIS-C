/*
 * A354354  a(n) = 1 if n is neither a multiple of 2 nor 3, and otherwise 0.
 * https://oeis.org/A354354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 if n is neither a multiple of 2 nor 3, otherwise 0
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 9917 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return ((n % 2) != 0 && (n % 3) != 0) ? 1 : 0;
}
