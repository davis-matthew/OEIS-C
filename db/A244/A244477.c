/*
 * A244477  a(1)=3, a(2)=2, a(3)=1; thereafter a(n) = a(n-a(n-1)) +
 *          a(n-a(n-2)).
 * https://oeis.org/A244477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-2 if n mod 3 == 0 * a(n) = 3 if n mod 3 == 1 * a(n) = n
 *            if n mod 3 == 2
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n % 3 == 0) {
        return (seq_t)(n - 2);
    } else if (n % 3 == 1) {
        return (seq_t)3;
    } else { /* n % 3 == 2 */
        return (seq_t)n;
    }
}
