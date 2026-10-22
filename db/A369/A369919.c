/*
 * A369919  Triangular array read by rows. T(n,k) is the number of labeled
 *          posets on [n] of rank at most one with exactly k elements of
 *          positive indegree, n >= 0, 0 <= k <= max{0,n-1}.
 * https://oeis.org/A369919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where n is the (n‑th) element in the flattened
 *            triangle with rows indexed by r and columns by k. T(r,k) =
 *            C(r,k) * (2^{r-k} - 1)^k where r is the smallest integer with
 *            r(r+1)/2 >= n. This implements the OEIS sequence A369919
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom, pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n.  For n=0 this yields r=0. */
    idx_t r = 0;
    while (r * (r + 1) / 2 < n) {
        ++r;
    }

    /* Previous cumulative count up to row r-1 */
    idx_t prev_cum = (r == 0) ? 0 : 1 + (r - 1) * r / 2;

    /* Column index k within the row */
    idx_t k = n - prev_cum;

    /* Compute binomial coefficient C(r,k) */
    seq_t bin = binom(r, k);

    /* Compute (2^{r-k} - 1) */
    seq_t base = pow2(r - k);
    base = base - (seq_t)1;

    /* Compute (base)^k, handling k==0 specially to ensure 0^0 == 1 */
    seq_t power;
    if (k == 0) {
        power = (seq_t)1;
    } else {
        power = pow_seq(base, (int)k);
    }

    /* Result is binomial times power */
    return bin * power;
}
