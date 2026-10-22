/*
 * A367178  Triangle read by rows. T(n, k) = binomial(n, k)^2 *
 *          CatalanNumber(k).
 * https://oeis.org/A367178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r,k)^2 * catalan(k) where r is the largest
 *            integer * such that r*(r+1)/2 <= n, and k = n - r*(r+1)/2. * The
 *            triangle T(r,k) = binomial(r,k)^2 * catalan(k) is read by rows
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    idx_t r = 0;
    idx_t cum = 0;
    /* Find the row r that contains the n-th term.  cum is the total
     * number of entries before row r.  */
    while (cum + r + 1 <= n) {
        cum += r + 1;
        ++r;
    }
    /* Column k within that row. */
    idx_t k = n - cum;
    /* Compute T(r,k). */
    seq_t b = binomial(r, k);
    seq_t c = catalan(k);
    return (b * b) * c;
}
