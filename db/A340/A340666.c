/*
 * A340666  A(n,k) is derived from n by replacing each 0 in its binary
 *          representation with a string of k 0's; square array A(n,k), n>=0,
 *          k>=0, read by antidiagonals.
 * https://oeis.org/A340666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, look, base
 * Formula:   a(n) = A(i,j) where (i,j) is the element at anti‑diagonal *
 *            position n in the square array defined by the OEIS entry for
 *            A340666
 * Range:     u8 n <= 39, u16 n <= 81, u32 n <= 197, u64 n <= 472,
 *            u128 n <= 1191, big any n
 * Validated: standard, pass, 94 terms through n = 9689 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* Build A(n,k): replace each 0 in the binary representation of n by k zeros. */
static seq_t A_of_n_k(idx_t n, idx_t k)
{
    if (n == 0)
        return 0;

    /* Find most significant bit index of n. */
    idx_t msb = 0;
    idx_t t   = n;
    while ((t >>= 1) != 0)
        ++msb;

    seq_t pow2k = pow2(k);
    seq_t res   = 0;

    for (int pos = (int)msb; pos >= 0; --pos)
    {
        if ((n >> pos) & 1u)
            res = res * 2 + 1;
        else
            res = res * pow2k;
    }

    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find the anti‑diagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2. */
    idx_t low  = 0;
    idx_t high = n;
    while (low < high)
    {
        idx_t mid = low + (high - low + 1u) / 2u;
        seq_t cum = (seq_t)mid * (mid + 1u) / 2u;
        if (cum <= n)
            low = mid;
        else
            high = mid - 1u;
    }
    idx_t s = low;

    seq_t cum_before = (seq_t)s * (s + 1u) / 2u;
    idx_t i = n - (idx_t)cum_before;         /* row index */
    idx_t k = s - i;                          /* column index */

    return A_of_n_k(i, k);
}
