/*
 * A355202  Square array read by upwards antidiagonals: T(n,k) = k-th binary
 *          digit after the radix point of 1/n, for n >= 1 and k >= 1.
 * https://oeis.org/A355202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   a(n) = T(n_row, k) where (n_row, k) is obtained by reading the
 *            square array T(n,k) (k-th binary digit after the point of 1/n)
 *            by upwards antidiagonals
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined, return 0 */

    /* Find the diagonal sum s such that (s-1)*s/2 >= n */
    idx_t s = 2;
    while (((unsigned __int128)(s - 1) * s) / 2 < n) ++s;

    /* Number of terms before this diagonal */
    idx_t prev = ((unsigned __int128)(s - 2) * (s - 1)) / 2;
    /* 0–based offset within the diagonal */
    idx_t offset = n - prev - 1;

    /* Row (n) and column (k) of the required digit */
    idx_t n_row = s - 1 - offset; /* n ≥ 1 */
    idx_t k = offset + 1;         /* k ≥ 1 */

    /* Compute digit: T(n_row, k) = 1 iff 2*(2^(k-1) mod n_row) >= n_row */
    if (n_row == 0) return (seq_t)0; /* safety, not normally reached */

    idx_t r = 1 % n_row;                 /* 2^(0) mod n_row */
    for (idx_t i = 0; i < k - 1; ++i) {  /* multiply by 2 (k-1) times */
        unsigned __int128 temp = (unsigned __int128)r * 2;
        r = (idx_t)(temp % n_row);
    }

    return (seq_t)((2 * r) >= n_row ? 1 : 0);
}
