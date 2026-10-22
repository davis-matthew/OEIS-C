/*
 * A368745  Triangular array read by rows: T(n, k) is the number of n X 2
 *          Young tableaux with k vertical walls.
 * https://oeis.org/A368745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n, k) = binomial(n, k) * binomial(2*n, n) / (n + 1 - k). The
 *            sequence a(n) is the triangular array read by rows. a(n) = T(r,
 *            k) where r is the largest integer with r*(r+1)/2 ≤ n and k = n –
 *            r*(r+1)/2
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 109, u64 n <= 334,
 *            u128 n <= 1146, big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    /* use 128‑bit arithmetic for the comparison to avoid overflow */
    while ((unsigned __int128)r * (r + 1) / 2 <= n) {
        r++;
    }
    r--;  /* now r satisfies r*(r+1)/2 ≤ n */

    /* Column k in that row */
    idx_t tri = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - tri;

    /* Compute the triangle entry T(r, k). */
    seq_t bin1 = binomial(r, k);
    seq_t bin2 = binomial(2 * r, r);
    seq_t denom = (seq_t)(r + 1 - k);

    /* Reduce the denominator with bin1 and bin2 to keep intermediate
       values small and avoid overflow for small seq_t types. */
    seq_t g = gcd(bin1, denom);
    if (g != 0) {
        bin1 = bin1 / g;
        denom = denom / g;
    }
    g = gcd(bin2, denom);
    if (g != 0) {
        bin2 = bin2 / g;
        denom = denom / g;
    }

    /* Now compute bin1 * bin2 / denom. */
    seq_t prod = bin1 * bin2;   /* safe after reduction for supported seq_t */
    seq_t result = prod / denom;
    return result;
}
