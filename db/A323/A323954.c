/*
 * A323954  Regular triangle read by rows where T(n, k) is the number of ways
 *          to split an n-cycle into connected subsequences of sizes > k, n
 *          >=1, 0 <= k < n.
 * https://oeis.org/A323954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle read by rows of T(n,k) with T(n,k) = 1 - n
 *            + Sum_{i=1..floor(n/(k+1))} n*binomial(n-i*k-1,i-1)/i
 * Range:     u8 n <= 28, u16 n <= 136, u32 n <= 528, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility: binomial coefficients (passed through the utils directory) */
#include <math.h>
#include "../utils/binomial.h"

/* Computes a(n) for n >= 1.  Returns 0 for n == 0 (not part of the range). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Find the row r such that r*(r+1)/2 >= n. */
    double dn = (double)n;
    idx_t r = (idx_t)((sqrt(8.0 * dn + 1.0) - 1.0) / 2.0);

    /* Adjust r in case the floating‑point guess was off. */
    while (((unsigned __int128)r * (r + 1)) / 2 < n) ++r;
    while (r > 0 && (((unsigned __int128)(r - 1) * r) / 2) >= n) --r;

    /* Compute column k within row r. */
    idx_t prev_total = (unsigned __int128)r * (r - 1) / 2;
    idx_t k = n - prev_total - 1;   /* k runs from 0 to r-1 */

    /* Compute T(r,k). */
    idx_t m = r / (k + 1);           /* floor(r/(k+1)) */
    seq_t sum = 0;

    for (idx_t i = 1; i <= m; ++i)
    {
        idx_t nR = r - i * k - 1;    /* upper part of binomial */
        idx_t kR = i - 1;            /* lower part of binomial */

        seq_t bin = binomial(nR, kR);                /* binomial(nR, kR) */
        seq_t term = bin * (seq_t)r;                 /* n * binom(...)   */
        term = term / (seq_t)i;                      /* divide by i      */
        sum += term;                                 /* accumulate sum   */
    }

    seq_t result = sum + 1 - (seq_t)r;   /* 1 - r + sum   */
    return result;
}
