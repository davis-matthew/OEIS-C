/*
 * A273183  The H_3-Eulerian numbers.
 * https://oeis.org/A273183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1 for n=0 or 3, 59 for n=1 or 2, otherwise 0
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0ULL || n == 3ULL) return (seq_t)1;
    if (n == 1ULL || n == 2ULL) return (seq_t)59;
    return (seq_t)0;
}
