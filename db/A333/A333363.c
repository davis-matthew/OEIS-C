/*
 * A333363  Horizontal visibility sequence at the onset of chaos in the
 *          3-period cascade.
 * https://oeis.org/A333363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2*A007814(n/3) + 5 if 3|n, otherwise a(n) = 4 - (n mod 3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if ((n % 3) == 0) {
        idx_t k = n / 3;
        unsigned int exp = 0;
        while ((k & 1) == 0) {
            exp++;
            k >>= 1;
        }
        return (seq_t)(2 * exp + 5);
    } else {
        return (seq_t)(4 - (n % 3));
    }
}
