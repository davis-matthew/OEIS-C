/*
 * A357498  Triangle read by rows where each term in row n is the next greater
 *          multiple of n..1 divided by n..1.
 * https://oeis.org/A357498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the element T(r,c) of a triangle defined by * T(r,1) =
 *            1, * T(r,k) = (T(r,k-1)*(r-k+2) + (r-k+1) - (T(r,k-1)*(r-k+2) %
 *            (r-k+1))) / (r-k+1) * for 2 ≤ k ≤ r, where r is the smallest
 *            integer with r(r+1)/2 ≥ n and * c = n - r(r-1)/2
 * Range:     u8 n <= 377, u16 n <= 9546, u32 n <= 9546, u64 n <= 9546,
 *            u128 n <= 9546, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t N) {
    /* Convert to 64‑bit local variables, if needed. */
    idx_t n = N;

    /* Locate the row r using the quadratic formula. */
    long double val = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((val - 1.0L) / 2.0L);
    while (r * (r + 1) / 2 < n) ++r;          /* safety for rounding errors */

    /* Index of the element inside the row. */
    idx_t prev_rows = r * (r - 1) / 2;
    idx_t k = n - prev_rows;                  /* 1 ≤ k ≤ r */

    /* Compute the triangle entry via the recurrence. */
    seq_t val01 = 1;                          /* T(r,1) */
    for (idx_t col = 2; col <= k; ++col) {
        idx_t m = r - col + 1;                /* divisor (r - k + 1) */
        idx_t num = r - col + 2;              /* multiplier (r - k + 2) */
        seq_t prod = val01 * (seq_t)num;
        seq_t rem = prod % (seq_t)m;           /* remainder */
        val01 = (prod + (seq_t)m - rem) / (seq_t)m; /* next multiple */
    }
    return val01;
}
