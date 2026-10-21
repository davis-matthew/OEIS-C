/*
 * A289705  Number of 4-cycles in the n-triangular honeycomb queen graph.
 * https://oeis.org/A289705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (24*n^5 + 170*n^4 - 660*n^3 + 160*n^2 + 606*n - 165 * +
 *            (-1)^n * (165 - 30*n)) / 320
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Returns the n-th term of OEIS A289705 (index n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1, return 0 */
    /* Use signed 128‑bit arithmetic for intermediate values. */
    __int128 N = (unsigned long long)n;
    __int128 num = 24 * N * N * N * N * N;   /* 24*n^5 */
    num += 170 * N * N * N * N;              /* +170*n^4 */
    num -= 660 * N * N * N;                  /* -660*n^3 */
    num += 160 * N * N;                      /* +160*n^2 */
    num += 606 * N;                          /* +606*n   */
    num -= 165;                              /* -165     */

    /* Add the (-1)^n * (165 - 30*n) term */
    __int128 c = 165 - 30 * N;
    if (n & 1)    /* n is odd -> (-1)^n = -1 */
        num -= c;
    else          /* n is even -> (-1)^n = +1 */
        num += c;

    /* Result is an integer; divide by 320. */
    seq_t res = (seq_t)(num / 320);
    return res;
}
