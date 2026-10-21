/*
 * A257164  Period 5 sequence: repeat [0, 2, 4, 1, 3].
 * https://oeis.org/A257164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n mod 5
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert the index to a seq_t, multiply by 2, and reduce modulo 5. */
    seq_t val = (seq_t)n * (seq_t)2;
    return val % (seq_t)5;
}
