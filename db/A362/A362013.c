/*
 * A362013  Triangular array read by rows. T(n,k) is the number of labeled
 *          directed graphs on [n] with exactly k strongly connected
 *          components of size 1 with outdegree zero, n>=0, 0<=k<=n.
 * https://oeis.org/A362013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8n+1)−1)/2⌋ and k = n−r(r+1)/2;
 *            T(r,k) = C(r,k) * (2^(r-1)−1)^(r−k) (r≥0, 0≤k≤r)
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 27, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 0, the triangular array entry is T(0,0)=1. */

#include "../utils/binom.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;       /* base case: a(0)=1 */

    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Local index k inside the row. */
    idx_t k = n - r * (r + 1) / 2;

    /* Base for the power: 2^(r-1) – 1.  For r=1 this is 0. */
    seq_t base = ((seq_t)1 << (r - 1)) - 1;

    /* Compute (2^(r-1)-1)^(r-k). */
    seq_t pow = big_pow(base, r - k);

    /* Compute binomial coefficient C(r,k). */
    seq_t comb = binom(r, k);

    /* Final result. */
    return comb * pow;
}
