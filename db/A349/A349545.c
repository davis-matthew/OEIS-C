/*
 * A349545  Triangular array read by rows: T(n,k) =
 *          A002884(k)*2^((n-k)(n-k-1)), n >= 0, 0 <= k <= n.
 * https://oeis.org/A349545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = GL(k,2) * 2^((r-k)(r-k-1)), where (r,k) is the row and
 *            column of the triangular array: r = floor( (sqrt(8n+1)-1)/2 ), k
 *            = n - r(r+1)/2. GL(k,2) = ∏_{i=0}^{k-1} (2^k - 2^i)
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 26, u64 n <= 44, u128 n <= 77,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Find the row number r for a given index n in the triangular array. */
static idx_t row_of_index(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;

    /* Double high until the cumulative count exceeds n, guarding
       against overflow by checking high <= n. */
    while (high <= n &&
           ((unsigned __int128)high * (high + 1) / 2) <= n)
    {
        high <<= 1;
    }

    /* Binary search in the interval [low, high). */
    while (low + 1 < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (((unsigned __int128)mid * (mid + 1) / 2) <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Compute the value GL(k,2) = product_{i=0}^{k-1} (2^k - 2^i). */
static seq_t gl_k(idx_t k)
{
    seq_t prod = (seq_t)1;
    if (k == 0)
        return prod;

    seq_t two_pow_k = pow2(k);
    for (idx_t i = 0; i < k; ++i)
    {
        seq_t term = two_pow_k - pow2(i);
        prod *= term;
    }
    return prod;
}

/* Main entry: a(n) */
seq_t a(idx_t n)
{
    /* Determine the row r and column k for index n. */
    idx_t r = row_of_index(n);
    /* offset = r(r+1)/2 */
    idx_t offset = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t k = n - offset;

    /* Compute GL(k,2). */
    seq_t gl_val = gl_k(k);

    /* Compute the exponent (r-k)(r-k-1). */
    idx_t diff = r - k;
    unsigned __int128 exp128 = 0;
    if (diff > 0)
        exp128 = (unsigned __int128)diff * (diff - 1);
    idx_t exponent = (idx_t)exp128;

    /* 2^exponent */
    seq_t pow_val = pow2(exponent);

    /* Final result. */
    seq_t result = gl_val * pow_val;
    return result;
}
