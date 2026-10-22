/*
 * A317504  Triangle read by rows: T(0,0) = 1; T(n,k) = 2*T(n-1,k) -
 *          T(n-3,k-1) for k = 0..floor(n/3); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A317504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 44, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A317504:  Triangle read by rows
   T(n,k) = 2 * T(n‑1,k) − T(n‑3,k‑1),  T(0,0)=1
   a(n) is the flattening of the triangle by rows.

   Formula used:  a(n) = (-1)^k * 2^(n-3k) * binomial(n-2k, k)
   where k is the column index of the element with row n in the flattened triangle.
*/

#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* Find the row (r) and column (k) corresponding to the n-th term in the flattening. */
    idx_t r = 0;          /* current row */
    idx_t count = 0;      /* number of terms seen up to previous row */

    while (1) {
        idx_t len = r / 3 + 1;            /* number of elements in this row */
        if (count + len > n) break;        /* the target term is in this row */
        count += len;
        ++r;
    }

    idx_t k = n - count;      /* column index within row r */

    /* Compute the value using the closed‑form formula. */
    seq_t val = binomial(r - 2*k, k);          /* binomial(n-2k, k) */
    val = val * pow2(r - 3*k);                  /* multiply by 2^(n-3k) */
    if (k & 1U) val = -val;                    /* multiply by (-1)^k */

    return val;
}
