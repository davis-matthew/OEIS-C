/*
 * A271913  Number of ways to choose three distinct points from a 4 X n grid
 *          so that they form an isosceles triangle.
 * https://oeis.org/A271913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of isosceles triangles formed by * 3 distinct
 *            points in a 4×n grid. * * For n ≤ 8 this value is computed by
 *            brute force Enumeration. * For n > 8 it follows the closed‑form:
 *            * even n : 5*n^2 + 64*n - 216 * odd n : 5*n^2 + 64*n - 213 * *
 *            This matches the OEIS recurrence a(n)=2*a(n-1)-...
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Very small grids are handled by brute force enumeration. */
    if (n <= 8ULL) {
        const idx_t N = 4 * n;
        seq_t count = 0;
        for (idx_t i = 0; i < N - 2; ++i) {
            idx_t r1 = i % 4;
            idx_t c1 = i / 4;
            for (idx_t j = i + 1; j < N - 1; ++j) {
                idx_t r2 = j % 4;
                idx_t c2 = j / 4;
                /* differences for cross product test */
                int64_t dx12 = (int64_t)r2 - (int64_t)r1;
                int64_t dy12 = (int64_t)c2 - (int64_t)c1;
                for (idx_t k = j + 1; k < N; ++k) {
                    idx_t r3 = k % 4;
                    idx_t c3 = k / 4;

                    /* Check non‑collinearity */
                    int64_t dx13 = (int64_t)r3 - (int64_t)r1;
                    int64_t dy13 = (int64_t)c3 - (int64_t)c1;
                    if (dx12 * dy13 - dy12 * dx13 == 0)
                        continue;

                    /* Squared distances */
                    seq_t d12 = (seq_t)(dx12 * dx12) + (seq_t)(dy12 * dy12);
                    seq_t d13 = (seq_t)(dx13 * dx13) + (seq_t)(dy13 * dy13);
                    int64_t dr23 = (int64_t)r3 - (int64_t)r2;
                    int64_t dc23 = (int64_t)c3 - (int64_t)c2;
                    seq_t d23 = (seq_t)(dr23 * dr23) + (seq_t)(dc23 * dc23);

                    /* Isosceles if any two sides equal and non‑degenerate */
                    if (d12 == d13 || d12 == d23 || d13 == d23)
                        ++count;
                }
            }
        }
        return count;
    }

    /* Closed‑form for n > 8 (works for all n ≥ 9). */
    seq_t nn = (seq_t)n;
    seq_t val = 5 * nn * nn + 64 * nn;
    if ((n & 1) == 0) {          /* n is even */
        val -= 216;
    } else {                     /* n is odd  */
        val -= 213;
    }
    return val;
}
