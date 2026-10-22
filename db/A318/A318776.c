/*
 * A318776  Triangle read by rows: T(0,0) = 1; T(n,k) = 2*T(n-1,k) +
 *          T(n-5,k-1) for k = 0..floor(n/5); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A318776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 33, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows:
   a(n) = T(r,k) where r is the row containing the n-th term
   T(r,k) = 2^(r-5*k) * binom(r-4*k, k)
   row r has floor(r/5)+1 entries.
   Uses the provided binom(idx_t a, idx_t b) and pow2(idx_t exp).
*/

#include "../utils/binom.h"   /* binom() */
#include "../utils/pow2.h"    /* pow2() */

seq_t a(idx_t n)
{
    idx_t row = 0;          /* current row index */
    idx_t cum = 0;          /* cumulative count of terms before current row */

    /* Find the row that contains the n-th term. */
    while (1) {
        idx_t cnt = row / 5 + 1;            /* number of entries in this row */
        if (cum + cnt > n) break;           /* this row contains the term */
        cum += cnt;
        ++row;
    }

    idx_t k   = n - cum;                     /* column index within the row */
    idx_t exp = row - 5 * k;                 /* exponent for the power of 2 */
    idx_t bin_idx = row - 4 * k;             /* argument for the binomial */

    seq_t val = binom(bin_idx, k);
    val *= pow2(exp);
    return val;
}
