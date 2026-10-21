/*
 * A263615  Partial sums of A263614 starting at n=2.
 * https://oeis.org/A263615
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) =
 *            (2*n*(3*n^3-14*n^2+147*n+272)+(4*n^3-30*n^2+128*n-27)*(-1)^n-741)/768
 * Range:     u8 n <= 13, u16 n <= 54, u32 n <= 862, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* a(2) is the first term. For n < 2 the value is undefined in OEIS, return 0. */
    if (n < 2) return (seq_t)0;

    /* Perform all intermediate arithmetic in signed 64‑bit integers. */
    int64_t N   = (int64_t)n;
    int64_t n2  = N * N;          /* n^2 */
    int64_t n3  = n2 * N;         /* n^3 */

    int64_t term1 = 2 * N * (3 * n3 - 14 * n2 + 147 * N + 272);

    int64_t base = 4 * n3 - 30 * n2 + 128 * N - 27;
    /* (−1)^n : add base for even n, subtract for odd n. */
    int64_t term2 = (n & 1) ? -base : base;

    int64_t num = term1 + term2 - 741;

    /* The division is exact for integer n ≥ 2. */
    return (seq_t)(num / 768);
}
