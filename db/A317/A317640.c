/*
 * A317640  The 7x+-1 function: a(n) = 7n+1 if n == +1 (mod 4), a(n) = 7n-1 if
 *          n == -1 (mod 4), otherwise a(n) = n/2.
 * https://oeis.org/A317640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n+1 if n == +1 (mod 4), a(n) = 7*n-1 if n == -1 (mod
 *            4), a(n) = n/2 otherwise
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if ((n & 3) == 1ULL) {
        return (seq_t)7 * (seq_t)n + 1;
    }
    if ((n & 3) == 3ULL) {
        return (seq_t)7 * (seq_t)n - 1;
    }
    return (seq_t)(n >> 1);
}
