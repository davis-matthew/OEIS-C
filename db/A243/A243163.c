/*
 * A243163  Triangle read by rows: T(n,k) =
 *          binomial(k,n-k)*binomial(n+2*k,n+k) /(n+k+1), n>=0, 0<=k<=n.
 * https://oeis.org/A243163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r(r+1)/2 ≤ n < (r+1)(r+2)/2, r =
 *            floor((√(8n+1)-1)/2), k = n - r(r+1)/2, T(r,k) = binomial(k,r-k)
 *            * binomial(r+2k,r+k) / (r+k+1) (the triangle is read row‑by‑row
 *            and flattened into a sequence)
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index k within the row */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Compute the two binomial coefficients */
    seq_t bin1 = binomial(k, r - k);          /* C(k, r-k) */
    seq_t bin2 = binomial(r + 2 * k, r + k);  /* C(r+2k, r+k) */

    /* Denominator to be cancelled */
    seq_t d = (seq_t)(r + k + 1);

    /* Reduce by cancelling common factors with the denominator to avoid overflow */
    seq_t g = gcd(bin1, d);
    if (g > 1) {
        bin1 = bin1 / g;
        d    = d / g;
    }
    g = gcd(bin2, d);
    if (g > 1) {
        bin2 = bin2 / g;
        d    = d / g;
    }

    /* After cancellation, d should be 1; return the product */
    return bin1 * bin2;
}
