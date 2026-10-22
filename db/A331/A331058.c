/*
 * A331058  Coordination sequence for trivalent vertex in 1-skeleton of
 *          deltoidal icositetrahedron.
 * https://oeis.org/A331058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1 when n=0 or n=6; 3 when n=1 or n=5; 6 when n=2,3,4; 0
 *            otherwise
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0ULL || n == 6ULL) {
        return (seq_t)1;
    }
    if (n == 1ULL || n == 5ULL) {
        return (seq_t)3;
    }
    if (n == 2ULL || n == 3ULL || n == 4ULL) {
        return (seq_t)6;
    }
    return (seq_t)0;
}
