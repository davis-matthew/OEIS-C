/*
 * A304157  a(n) is the first Zagreb index of the linear phenylene G[n],
 *          defined pictorially in the Darafsheh reference.
 * https://oeis.org/A304157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 44*n - 20
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)44 * n - (seq_t)20;
}
