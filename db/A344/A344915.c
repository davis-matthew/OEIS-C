/*
 * A344915  T(n, k) = (3^(-k)*n!*2^(n - 3*k))/(k!*(n - 3*k)!), for n >= 0 and
 *          0 <= k <= floor(n/3). Triangle read by rows.
 * https://oeis.org/A344915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n, k) = (3^(-k) * n! * 2^(n-3k))/(k! * (n-3k)!)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 38, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   factorial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows; a(n) is the row‑major entry indexed from 0. */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Find the row r (starting at 0) and column k such that the
     * index n lies at T(r,k). The number of entries in row i is
     * floor(i/3)+1. */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum + (r / 3 + 1) <= n) {
        cum += (r / 3 + 1);
        ++r;
    }
    idx_t k = n - cum;

    /* Compute T(r,k) multiplicatively:
     *   T = (r! / (r-3k)! ) * 2^(r-3k) / 3^k / k!
     * The factor r!/(r-3k)! is a product of 3k consecutive integers. */
    seq_t prod = 1;
    idx_t start = r - 3 * k + 1;          /* r >= 3*k by definition */
    for (idx_t i = start; i <= r; ++i) {
        prod *= (seq_t)i;
    }
    prod *= pow2(r - 3 * k);
    prod /= pow3(k);
    prod /= factorial(k);

    return prod;
}
