/*
 * A256268  Table of k-fold factorials, read by antidiagonals.
 * https://oeis.org/A256268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the anti‑diagonal read of the infinite matrix * A(i,j) =
 *            ∏_{t=0}^{j-1} (1 + i·t) (with A(i,0)=1). This implements * the
 *            definition of OEIS A256268
 * Range:     u8 n <= 28, u16 n <= 46, u32 n <= 92, u64 n <= 192,
 *            u128 n <= 438, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 128‑bit unsigned values.  Returns ⌊√x⌋. */
static inline idx_t
isqrt_u128(unsigned __int128 x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)-1;        /* maximum 64‑bit value, sqrt fits in 64 bits */

    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 midsq = (unsigned __int128)mid * mid;
        if (midsq <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;   /* avoid underflow */
            hi = mid - 1;
        }
    }
    return lo - 1;
}

/* Computes the OEIS A256268 sequence value for index n. */
seq_t
a(idx_t n)
{
    /* The term a(0)=1 corresponds to the (0,0) entry. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Find the diagonal d such that d*(d+1)/2 ≤ n < (d+1)*(d+2)/2.
     * Solve d² + d - 2n ≤ 0  →  d = ⌊(√(1+8n) - 1)/2⌋.               */
    unsigned __int128 s = (unsigned __int128)8 * n + 1;
    idx_t d = (isqrt_u128(s) - 1) >> 1;   /* (sqrt(s)-1)/2 */

    /* Verify bounds to handle any rounding issues. */
    unsigned __int128 base128 = (unsigned __int128)d * (d + 1) / 2;
    idx_t base = (idx_t)base128;
    if (base > n) {
        d--;
        base128 = (unsigned __int128)d * (d + 1) / 2;
        base = (idx_t)base128;
    } else if (((unsigned __int128)(d + 1) * (d + 2) / 2) <= n) {
        d++;
        base128 = (unsigned __int128)d * (d + 1) / 2;
        base = (idx_t)base128;
    }

    idx_t offset = n - base;
    idx_t i = offset;        /* row index (original n in table)        */
    idx_t j = d - offset;    /* column index (original k in table)    */

    /* Compute the product A(i,j) = ∏_{t=0}^{j-1} (1 + i·t). */
    seq_t res   = (seq_t)1;
    seq_t seq_i = (seq_t)i;
    for (idx_t t = 0; t < j; ++t) {
        seq_t term = (seq_t)1 + seq_i * (seq_t)t;
        res *= term;
    }
    return res;
}
