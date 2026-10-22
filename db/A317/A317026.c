/*
 * A317026  Triangle read by rows: T(0,0) = 1; T(n,k) = T(n-1,k) + 8 *
 *          T(n-2,k-1) for k = 0..floor(n/2); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A317026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 39, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A317026
   The sequence a(n) lists the triangle T(n,k) defined by
        T(0,0) = 1
        T(n,k) = T(n-1,k) + 8*T(n-2,k-1)   (k = 0…⌊n/2⌋)
   In closed form:
        T(n,k) = 8^k * C(n-k, k)
   a(n) is obtained by writing the rows of the triangle one after another.
   For a given global index n one first finds the row r such that
   the number of terms before row r does not exceed n and the cumulative
   count up to row r is greater than n.  The intra‑row index is
   k = n – (#terms before row r).  The value returned is
        a(n) = 8^k * C(r-k, k).
*/

#include <stddef.h>                 /* for size_t */
#include "../utils/binomial.h"      /* binomial(idx_t, idx_t) returns seq_t */

seq_t a(idx_t n)
{
    /* Locate the row r containing the n‑th term and find the intra‑row index k. */
    idx_t r     = 0;       /* current row index   */
    idx_t count = 0;       /* number of terms before current row */

    while (1) {
        /* number of terms in row r: floor(r/2)+1 */
        idx_t len = (r / 2) + 1;
        if (count + len > n) break;   /* r is the target row */
        count += len;
        ++r;
    }

    idx_t k = n - count;   /* position within row r (0‑based) */

    /* Compute 8^k.  The power is small enough that a short loop is efficient. */
    seq_t eight_pow = 1;
    for (idx_t i = 0; i < k; ++i) {
        eight_pow *= (seq_t)8;
    }

    /* Binomial coefficient C(r-k, k).  Use the provided helper. */
    seq_t comb = binomial(r - k, k);

    return comb * eight_pow;
}
