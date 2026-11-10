/*
 * A356426  Even bisection of A003278.
 * https://oeis.org/A356426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003278(2n); i.e. a(n)-1 is the binary expansion of 2n-1
 *            read in base 3
 * Range:     u8 n <= 19, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A003278_a(idx_t n) {
    idx_t m = n - 1;
    seq_t result = 0;
    seq_t power = 1;
    while (m > 0) {
        if (m & 1) {
            result = result + power;
        }
        power = power * 3;
        m = m >> 1;
    }
    return result + 1;
}

seq_t a(idx_t n) {
    return A003278_a(2 * n);
}
