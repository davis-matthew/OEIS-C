/*
 * A344563  T(n, k) = binomial(n - 1, k - 1) * binomial(n, k) * 2^k, T(0, 0) =
 *          1. Triangle read by rows, T(n, k) for 0 <= k <= n.
 * https://oeis.org/A344563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where T(r, k) = binomial(r-1, k-1) * binomial(r,
 *            k) * 2^k, * with T(0,0) = 1 and T(r,0) = 0 for r > 0. The row
 *            index r and column * index k are obtained from the linear index
 *            n by finding the unique r such * that r*(r+1)/2 ≤ n <
 *            (r+1)*(r+2)/2. (The triangle is read row‑by‑row.)
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r containing the element with linear index n.
       The cumulative number of elements up to row r-1 is
       r*(r+1)/2.  We scan forward until this value exceeds n. */
    idx_t r = 0;
    idx_t cum = 0;                  /* cum = r*(r+1)/2 for the current r */
    while (cum + (r + 1) <= n) {
        cum += r + 1;               /* add the length of the next row */
        r++;                        /* move to the next row */
    }

    /* Compute the column index k within the found row */
    idx_t k = n - cum;              /* 0 ≤ k ≤ r */

    /* Handle the special rows/columns */
    if (r == 0)                     /* Only element (0,0) exists */
        return (k == 0) ? (seq_t)1 : (seq_t)0;

    if (k == 0)                     /* First element of any later row */
        return (seq_t)0;

    /* Compute T(r, k) = binomial(r-1,k-1) * binomial(r,k) * 2^k */
    seq_t a1 = binomial(r - 1, k - 1);
    seq_t a2 = binomial(r, k);
    seq_t pow2k = pow2(k);

    return (a1 * a2) * pow2k;
}
