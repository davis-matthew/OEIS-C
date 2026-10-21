/*
 * A256140  Square array read by antidiagonals upwards: T(n,k) = n^A000120(k),
 *          n>=0, k>=0.
 * https://oeis.org/A256140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry read by antidiagonals upwards from the square
 *            array T(n,k) = n^popcount(k), with rows indexed by n and columns
 *            by k
 * Range:     u8 n <= 111, u16 n <= 510, u32 n <= 5522, u64 n <= 9318,
 *            u128 n <= 9318, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine diagonal index d such that
       d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    double root = sqrt(8.0 * (double)n + 1.0);
    idx_t d = (idx_t)((root - 1.0) * 0.5);

    /* Correct possible rounding errors (using 128‑bit intermediate). */
    while ((__uint128_t)d * (d + 1) / 2 > n) --d;
    while ((__uint128_t)(d + 1) * (d + 2) / 2 <= n) ++d;

    /* Index of first element on diagonal d. */
    unsigned __int128 tri = (__uint128_t)d * (d + 1) / 2;
    idx_t start = (idx_t)tri;

    /* Position within diagonal. */
    idx_t offset = n - start;          /* 0 … d */
    idx_t n_coord = d - offset;        /* row index (n) */
    idx_t k_coord = offset;            /* column index (k) */

    /* popcount(k_coord).  Implementation uses the classic bit‑clear loop. */
    idx_t pop = 0;
    idx_t tmp = k_coord;
    while (tmp) {
        pop++;
        tmp &= tmp - 1;                /* clear lowest set bit */
    }

    /* Compute n_coord ^ pop (with 0^0 = 1). */
    seq_t result = 1;
    seq_t base = (seq_t)n_coord;
    for (idx_t i = 0; i < pop; ++i)
        result *= base;

    return result;
}
