/*
 * A395185  Triangle read by rows: T(n,k) = T(n-1,k-1) * T(n-1,k) for n >= 3,
 *          1 <= k <= n-1, with T(n,0) = T(n,n) = 1 and T(2,1) = 2.
 * https://oeis.org/A395185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, changed
 * Formula:   T(n,k) = 2^binomial(n-2,k-1) for n>=2, 1<=k<=n-1;
 *            T(n,0)=T(n,n)=1. a(n) is the nth element of the triangle read
 *            row by row
 * Range:     u8 n <= 30, u16 n <= 39, u32 n <= 48, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"


seq_t a(idx_t n)
{
    /* Locate the row r that contains the n-th element.
       Row r contains r+1 entries and
       the first element of row r has index r*(r+1)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t prefix = r * (r + 1) / 2;   /* elements before row r   */
    idx_t k = n - prefix;             /* column index (0..r)      */

    /* Boundary entries are always 1. */
    if (k == 0 || k == r)
        return (seq_t)1;

    /* Interior entries: T(r,k) = 2 ^ binomial(r-2, k-1).   */
    idx_t exp_n = r - 2;
    idx_t exp_k = k - 1;
    seq_t exponent = binomial(exp_n, exp_k);

    return big_pow((seq_t)2, exponent);
}
