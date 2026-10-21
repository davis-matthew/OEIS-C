/*
 * A249910  Digital root of A003500(n).
 * https://oeis.org/A249910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digital root of (2 + sqrt(3))^n + (2 - sqrt(3))^n. * The
 *            sequence is periodic with period 6: 2, 4, 5, 7, 5, 4, …
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const seq_t period[6] = {2, 4, 5, 7, 5, 4};
    return period[n % 6];
}
