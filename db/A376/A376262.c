/*
 * A376262  Triangle T read by rows: T(n, k) = (n^2 - 4*n - (-1)^n * (n - 4))
 *          / 2 + 4*k - (-1)^n * (1 + (-1)^k).
 * https://oeis.org/A376262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where * r = smallest integer with r*(r+1)/2 ≥ n
 *            (row number) * k = n - (r-1)*r/2 (column within the row) *
 *            T(r,k) = (r^2 - 5r + 4)/2 + 4k - 2 if r even and k even * (r^2 -
 *            5r + 4)/2 + 4k if r even and k odd * (r^2 - 3r - 4)/2 + 4k + 2
 *            if r odd and k even * (r^2 - 3r - 4)/2 + 4k if r...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 >= n.  r is at most O(sqrt(n)). */
    idx_t r = 0;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Position within the row. */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;                 /* 1 ≤ k ≤ r */

    /* Signed intermediates for the formula. */
    long long r_l = (long long)r;
    long long k_l = (long long)k;
    long long A;                          /* numerator of first part */
    if (r & 1) {                          /* r odd   →  r^2 - 3r - 4 */
        A = r_l * r_l - 3 * r_l - 4;
    } else {                              /* r even  →  r^2 - 5r + 4 */
        A = r_l * r_l - 5 * r_l + 4;
    }

    long long part1 = A / 2;              /* integer division truncates toward zero */

    long long part2;
    if (k & 1) {                          /* k odd   → 4k */
        part2 = 4 * k_l;
    } else {                              /* k even  */
        if (r & 1) {                      /* r odd   → 4k + 2 */
            part2 = 4 * k_l + 2;
        } else {                          /* r even  → 4k - 2 */
            part2 = 4 * k_l - 2;
        }
    }

    long long val = part1 + part2;        /* final value, always positive */
    return (seq_t)val;
}
