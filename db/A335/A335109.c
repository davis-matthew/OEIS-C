/*
 * A335109  Triangle read by rows: T(n,k) is the number of permutations of
 *          length n with each cycle of the permutation containing only
 *          elements that are identical (mod k), where 1 <= k <= n.
 * https://oeis.org/A335109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) reads the triangle T(n,k) by rows, where T(n,k) =
 *            (ceil(n/k)!)^(n mod k) * (floor(n/k)!)^(k - (n mod k)) (1≤k≤n)
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n */
    idx_t r = (idx_t)((sqrt(1.0 + 8.0 * n) - 1.0) / 2.0);
    while (r * (r + 1) / 2 < n) ++r;          /* safety adjustment */
    idx_t prev_tri = (r - 1) * r / 2;          /* t_{r-1} */
    idx_t k = n - prev_tri;                    /* column (1‑based) */

    /* Compute T(r,k) */
    idx_t floor_part = r / k;
    idx_t ceil_part  = (r + k - 1) / k;        /* ceil(r/k) */
    idx_t mod = r % k;

    /* factorial of floor_part */
    seq_t fact_floor = 1;
    for (idx_t i = 1; i <= floor_part; ++i)
        fact_floor *= (seq_t)i;

    /* factorial of ceil_part */
    seq_t fact_ceil = 1;
    for (idx_t i = 1; i <= ceil_part; ++i)
        fact_ceil *= (seq_t)i;

    /* result = fact_ceil^mod * fact_floor^(k-mod) */
    seq_t result = 1;
    for (idx_t i = 0; i < mod; ++i)
        result *= fact_ceil;
    for (idx_t i = 0; i < (k - mod); ++i)
        result *= fact_floor;

    return result;
}
