/*
 * A322229  Triangle, read by rows, where row n lists coefficients in
 *          Product_{k=1..n} (k + x + (n+1-k)*x^2), for n >= 0.
 * https://oeis.org/A322229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 107, u64 n <= 296,
 *            u128 n <= 799, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A322229 flattened sequence:  coefs of ∏_{k=1..r} (k + x + (r+1‑k)x²)
 *  Flattened index n → row r = ⌊√n⌋, col c = n – r² (symmetry: if c>r, set c←2r‑c).
 *  The coefficient T(r,c) is computed by dynamic programming in O(r·c) time.
 *  No external libraries are used aside from malloc/fre noC. */

#include <stdlib.h>   /* malloc, free */
#include <stdint.h>   /* uint64_t */

/* floor(sqrt(n)) for non‑negative n < 2⁶⁴ */
static idx_t _floor_isqrt_u64(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1ULL << 32;   /* 2³² > √(2⁶⁴) */
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Map flat index to row r and column c. */
    idx_t r = _floor_isqrt_u64(n);
    idx_t c = n - r * r;          /* 0 ≤ c ≤ 2r */
    if (c > r)                    /* use symmetry to reduce range */
        c = 2 * r - c;

    /* DP array of size c+1 (indices 0..c). */
    size_t len = (size_t)c + 1;
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp) return (seq_t)0;     /* allocation failure (unlikely) */

    dp[0] = (seq_t)1;
    for (size_t j = 1; j < len; ++j)
        dp[j] = (seq_t)0;

    /* Multiply successive factors. */
    for (idx_t i = 1; i <= r; ++i) {
        seq_t iw = (seq_t)i;                    /* coefficient of x⁰   */
        seq_t tw = (seq_t)(r + 1 - i);           /* coefficient of x²   */

        /* Update dp in reverse order to keep previous values. */
        for (idx_t j = len; j-- > 0;) {
            seq_t val = dp[j] * iw;
            if (j >= 1)
                val += dp[j - 1];
            if (j >= 2)
                val += dp[j - 2] * tw;
            dp[j] = val;
        }
    }

    seq_t result = dp[c];
    free(dp);
    return result;
}
