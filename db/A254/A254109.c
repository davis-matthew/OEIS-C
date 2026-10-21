/*
 * A254109  If n <= 63, a(n) = n; for n > 63: a(32n + 14) = 8*n + 5, a(64n +
 *          30) = 4*n + 3, and for other cases with n > 63: a(2n) = 2*a(n),
 *          a(2n+1) = 2*a(n) + 1.
 * https://oeis.org/A254109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n for 0 ≤ n ≤ 63; a(32 n + 14) = 8 n + 5; a(64 n + 30) =
 *            4 n + 3; a(2 n) = 2 a(n); a(2 n+1) = 2 a(n) + 1 (for n > 63)
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Direct identity for the first 64 terms. */
    if (n <= 63)
        return (seq_t)n;

    /* Rewrite rules that override the binary recursion. */
    if (n >= 14 && (n - 14) % 32 == 0) {
        idx_t m = (n - 14) / 32;
        return (seq_t)8 * m + 5;
    }
    if (n >= 30 && (n - 30) % 64 == 0) {
        idx_t m = (n - 30) / 64;
        return (seq_t)4 * m + 3;
    }

    /* Binary recursion for the remaining cases. */
    if ((n & 1) == 0)          /* n is even: n = 2*k */
        return (seq_t)2 * a(n >> 1);
    else                       /* n is odd:  n = 2*k + 1 */
        return (seq_t)2 * a(n >> 1) + 1;
}
