/*
 * A255908  Triangle read by rows: T(n,L) = number of rho-labeled graphs with
 *          n edges whose labeling is bipartite with boundary value L.
 * https://oeis.org/A255908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = T(r, L) where r is the row number such that (r-1)*r/2 < n ≤ r*(r+1)/2
   and L = n - (r-1)*r/2 - 1.
   T(r, L) = 2^r * L! * (L+1)^(r-L) */
   
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* find the row r that contains the nth entry */
    idx_t r = 1;
    while ( (r * (r + 1)) / 2 < n )
        ++r;

    /* column index (0‑based) within the row */
    idx_t prefix = (r - 1) * r / 2;
    idx_t L = n - prefix - 1;

    /* compute 2^r */
    seq_t pow2n = pow2(r);

    /* compute L! */
    seq_t fact = factorial(L);

    /* compute (L+1)^(r-L) */
    seq_t powBaseExp = 1;
    for (idx_t i = 0; i < r - L; ++i)
        powBaseExp *= (seq_t)(L + 1);

    /* final product */
    return pow2n * fact * powBaseExp;
}
