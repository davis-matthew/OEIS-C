/*
 * A382776  Triangle read by rows: T(n,k) is the number of ways to place 2*n
 *          rooks on a (n+k) X (2*n-k) board so that there is at least one
 *          rook in every column and row and so that each rook is defended by
 *          another.
 * https://oeis.org/A382776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row(n),col(n)) where T(n,k) = binomial(2*n-k,k) *
 *            binomial(n+k,n-k) * (2*(n-k))! * (2*k)! / 2^n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 27, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
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
    /* Find the row r and column k that correspond to the index n
       (0‑based indexing, triangle read by rows).                */
    idx_t r = 0;
    idx_t cum = 0;                 /* cumulative number of terms up to current row */
    while (1) {
        cum += r + 1;
        if (n < cum) break;
        ++r;
    }
    idx_t prev_cum = cum - (r + 1); /* number of terms before this row   */
    idx_t k = n - prev_cum;        /* column in this row                */

    /* Compute T(r,k) using the provided utilities.            */
    seq_t res = binomial(2 * r - k, k);
    res = res * binomial(r + k, r - k);
    res = res * factorial(2 * (r - k));
    res = res * factorial(2 * k);
    res = res / pow2(r);

    return res;
}
