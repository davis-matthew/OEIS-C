/*
 * A374434  Triangle read by rows: T(n, k) = Product_{p in PF(n) symmetric
 *          difference PF(k)} p, where PF(a) is the set of the prime factors
 *          of a.
 * https://oeis.org/A374434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row(n),col(n)), where T(i,j) is the product of primes
 *            that divide i or j but not both. It can be computed as
 *            (rad(i)/rad(gcd(i,j))) * (rad(j)/rad(gcd(i,j))) for i,j>0. The
 *            triangle is read by rows; row r has r+1 entries for k=0..r
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   gcd, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/rad.h"   /* seq_t rad(idx_t m); */
#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b); */

/* Convert a linear index to (row, col) in the triangle */
static void row_col_from_index(idx_t n, idx_t *row, idx_t *col)
{
    /* Solve r(r+1)/2 <= n < (r+1)(r+2)/2
       Using double arithmetic and correcting potential rounding errors. */
    double d = sqrt((double)8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) * 0.5);

    /* Correct any rounding issues */
    while ((((__int128)(r + 1) * (r + 2)) >> 1) <= n) ++r;
    while ((((__int128)r * (r + 1)) >> 1) > n) --r;

    *row = r;
    *col = n - (((__int128)r * (r + 1)) >> 1);
}

seq_t a(idx_t n)
{
    idx_t r, k;
    row_col_from_index(n, &r, &k);

    /* Special cases */
    if (r == 0 && k == 0)
        return (seq_t)1;

    if (k == 0)
        return rad(r);   /* rad(0) is undefined; for k==0 we return rad(r) */

    if (r == 0)
        return rad(k);

    /* General case: T(r,k) = (rad(r)/rad(g)) * (rad(k)/rad(g)),
       where g = gcd(r,k).  The division is exact. */
    seq_t rad_r = rad(r);
    seq_t rad_k = rad(k);
    idx_t g = (idx_t)gcd((seq_t)r, (seq_t)k);
    seq_t rad_g = rad(g);

    seq_t part1 = rad_r / rad_g;
    seq_t part2 = rad_k / rad_g;

    return part1 * part2;
}
