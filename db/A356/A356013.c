/*
 * A356013  Triangle T(n,k), n >= 1, 1 <= k <= n, read by rows, where T(n,k) =
 *          n!/(k! * floor(n/k)).
 * https://oeis.org/A356013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r!/(c!*floor(r/c)) where r is the unique integer with *
 *            r*(r+1)/2 ≥ n > (r-1)*r/2 and c = n - (r-1)*r/2
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t factorial(idx_t n) {
    seq_t res = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        res *= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* n≥1 per the offset, guard anyway */
    idx_t r = 1;
    /* Find the row r such that cumulative terms t(r) = r*(r+1)/2 ≥ n. */
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }
    idx_t t_prev = ((r - 1) * r) / 2;   /* cumulative terms up to row r-1 */
    idx_t c = n - t_prev;               /* column within the row */

    idx_t m = r / c;                    /* floor(r/c) */

    seq_t val = factorial(r);           /* r! */
    val /= (seq_t)m;                    /* divide by floor(r/c) */
    val /= factorial(c);                /* divide by c! */

    return val;
}
