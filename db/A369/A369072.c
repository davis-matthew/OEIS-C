/*
 * A369072  Triangle read by rows: T(n, k) = floor(binomial(n, k - 1) * (k -
 *          1)^(k - 1) * n * (n - k + 1)^(n - k) / 2).
 * https://oeis.org/A369072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = floor(C(n,k-1) * (k-1)^(k-1) * n * (n-k+1)^(n-k) / 2)
 *            a(n) = row‑major concatenation of the triangle T
 * Range:     u8 n <= 7, u16 n <= 26, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow, binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Determine the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    seq_t sq = floor_sqrt((seq_t)(8ULL * n + 1ULL));
    idx_t r = (idx_t)((sq - (seq_t)1) / (seq_t)2);

    /* Number of elements before this row */
    idx_t before = r * (r + 1ULL) / 2ULL;
    idx_t pos   = n - before;          /* position within the row (0‑based) */

    if (pos == 0ULL) {
        /* Leading zero column */
        return (seq_t)0;
    }

    /* pos is in [1 .. r]; k in the formula equals pos */
    idx_t k = pos;

    /* Binomial coefficient C(r, k-1) */
    seq_t binom_val = binomial(r, k - 1ULL);

    /* (k-1)^(k-1) and (r-k+1)^(r-k) using big_pow */
    seq_t pow1 = big_pow((seq_t)(k - 1ULL), (seq_t)(k - 1ULL));
    seq_t pow2 = big_pow((seq_t)(r - k + 1ULL), (seq_t)(r - k));

    /* Multiply all factors and divide by 2 (floor division) */
    seq_t val = binom_val * pow1 * (seq_t)r * pow2;
    val = val / (seq_t)2;

    return val;
}
