/*
 * A340342  a(n) = (671*n mod 2454) + (16*n mod 32) + (194*n mod 199).
 * https://oeis.org/A340342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = (671*n mod 2454) + (16*n mod 32) + (194*n mod 199)
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m1 = 2454, m2 = 32, m3 = 199;
    idx_t t1 = (n % m1) * 671 % m1;
    idx_t t2 = (n % m2) * 16 % m2;
    idx_t t3 = (n % m3) * 194 % m3;
    return (seq_t)(t1 + t2 + t3);
}
