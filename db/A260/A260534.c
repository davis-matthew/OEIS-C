/*
 * A260534  Square array read by ascending antidiagonals, T(n,k) =
 *          Sum_{j=0..k} n^j*(C(k-j,j) mod 2).
 * https://oeis.org/A260534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,k) where (m,k) are the coordinates in the square
 *            array read by ascending antidiagonals (rows in descending
 *            order). For a given index n: let s = floor((sqrt(8*n+1)-1)/2)
 *            let offset = n - s*(s+1)/2 let m = s - offset, k = offset Then
 *            T(m,k) = sum_{j=0..k} m^j * (binom(k-j,j) mod 2), wher...
 * Range:     u8 n <= 85, u16 n <= 199, u32 n <= 482, u64 n <= 1207,
 *            u128 n <= 3295, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Find diagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    __int128 prod = (__int128)8 * n + 1;
    long double d = sqrt((long double)prod);
    idx_t s = (idx_t)((d - 1.0L) / 2.0L);

    /* Correct possible rounding errors */
    while ((__int128)(s + 1) * (s + 2) / 2 <= (__int128)n) ++s;
    while ((__int128)s * (s + 1) / 2 > (__int128)n) --s;

    __int128 before = (__int128)s * (s + 1) / 2;
    idx_t offset = (idx_t)((__int128)n - before);
    idx_t row = s - offset;   /* m */
    idx_t col = offset;       /* k */

    /* Compute T(row, col) = sum_{j=0..col} row^j * (C(col-j,j) mod 2) */
    seq_t sum   = 0;
    seq_t power = 1;          /* row^0 initially */
    for (idx_t j = 0; j <= col; ++j) {
        if ((j & ~(col - j)) == 0) {  /* binom(col-j, j) mod 2 == 1 */
            sum += power;
        }
        power *= (seq_t)row;
    }
    return sum;
}
