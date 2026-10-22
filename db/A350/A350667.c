/*
 * A350667  Numbers congruent to 1, 3, and 8 modulo 9: positions of 1 in
 *          A159955.
 * https://oeis.org/A350667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = 1 + 9*k, a(3k+1) = 3 + 9*k, a(3k+2) = 8 + 9*k
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t k = n / 3;
    seq_t base = (seq_t)k;
    seq_t mul9 = base * 9;
    switch (n % 3) {
        case 0: return mul9 + 1;
        case 1: return mul9 + 3;
        default: return mul9 + 8;
    }
}
