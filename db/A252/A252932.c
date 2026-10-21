/*
 * A252932  Number of n X 2 nonnegative integer arrays with upper left 0 and
 *          every value within 3 of its king move distance from the upper left
 *          and every value increasing by 0 or 1 with every step right,
 *          diagonally se or down.
 * https://oeis.org/A252932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8/3)n^3 - 18n^2 + (145/3)n - 42 for n > 2 a(1) = 2, a(2)
 *            = 5
 * Range:     u8 n <= 6, u16 n <= 31, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return 2;
    if (n == 2) return 5;
    uint64_t N = (uint64_t)n;
    uint64_t n2 = N * N;           /* n² */
    uint64_t n3 = n2 * N;          /* n³ */
    uint64_t num = 8ULL * n3 + 145ULL * N; /* (8 n³ + 145 n) */
    uint64_t term = num / 3ULL;    /* (8 n³ + 145 n) / 3 */
    term -= 18ULL * n2;            /* subtract 18 n² */
    term -= 42ULL;                 /* subtract 42 */
    return (seq_t)term;
}
