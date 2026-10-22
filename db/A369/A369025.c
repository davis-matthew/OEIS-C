/*
 * A369025  Triangle read by rows: T(n, k) = floor(binomial(n, k - 1) * (k -
 *          1)^(k - 1) * k *(n - k + 1)^(n - k) / 2).
 * https://oeis.org/A369025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flatten(T, row‑major order) where T(n,k) = floor(
 *            binomial(n, k-1) * (k-1)^(k-1) * k * (n-k+1)^(n-k) / 2 ) with 0
 *            ≤ k ≤ n. This implementation maps the flattened index n to the
 *            corresponding row r and column c, then evaluates the closed form
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binom.h"

/* Integer sqrt for unsigned 128‑bit values, result fits in uint64_t. */
static inline idx_t isqrt_u128(unsigned __int128 v)
{
    idx_t lo = 0;
    idx_t hi = ((idx_t)1 << 34);          /* > sqrt(8·2^64+1) */
    while ((lo + 1) < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((unsigned __int128)mid * mid <= v)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;   /* 8n+1 may overflow 64‑bit */
    idx_t      m   = isqrt_u128(val);                     /* floor(sqrt(8n+1)) */
    idx_t      r   = (m - 1) / 2;                          /* r is the row index */

    /* Starting index of row r */
    unsigned __int128 start = (unsigned __int128)r * (r + 1) / 2;
    idx_t      c = n - (idx_t)start;                       /* column index within the row */

    if (c == 0) /* first element of each row is zero */
        return (seq_t)0;

    /* Compute the terms of the formula */
    seq_t bin = binom(r, c - 1);                           /* binomial(r, c-1) */
    seq_t pow1 = (c == 1) ? (seq_t)1 :
                 big_pow((seq_t)(c - 1), (seq_t)(c - 1)); /* (k-1)^(k-1) */
    seq_t pow2 = big_pow((seq_t)(r - c + 1), (seq_t)(r - c)); /* (n-k+1)^(n-k) */

    seq_t result = bin * pow1 * (seq_t)c * pow2 / (seq_t)2;
    return result;
}
