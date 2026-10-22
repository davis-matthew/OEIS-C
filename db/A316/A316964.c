/*
 * A316964  Same as A316669, except numbering of the squares starts at 0
 *          rather than 1.
 * https://oeis.org/A316964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + (k-1) if n = T_k (k≥1), a(n) = n if n = T_{k+1}−1,
 *            a(n) = n−1 otherwise, where T_k = k·(k+1)/2 and k =
 *            ⌊(√(8n+1)−1)/2⌋
 * Range:     u8 n <= 252, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for a 128‑bit unsigned integer using binary search.  */
static inline idx_t isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = (unsigned __int128)UINT64_MAX;
    unsigned __int128 mid, sq;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        sq = mid * mid;
        if (sq == x)                return (uint64_t)mid;
        if (sq < x)  lo = mid + 1;   else hi = mid - 1;
    }
    return (uint64_t)hi;  /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* a(0)=0 */

    /* Compute k = floor((sqrt(8*n+1)-1)/2). */
    unsigned __int128 d = (unsigned __int128)8 * n + 1;
    idx_t s = isqrt_uint128(d);
    idx_t k = (s - 1) >> 1;   /* k >= 1 for n > 0 */

    idx_t T_k  = k * (k + 1) / 2;  /* triangular number T_k */
    if (n == T_k) {                /* triangular position */
        /* For k = 1 (n = 1) we add 0; otherwise add (k-1). */
        if (k == 0) return (seq_t)n;     /* only occurs for n = 0 handled above */
        return (seq_t)n + (seq_t)(k - 1);
    }

    /* Not a triangular number. Determine if n is the last index before the next triangle. */
    idx_t T_next = (k + 1) * (k + 2) / 2;
    idx_t last   = T_next - 1;        /* index just before the next triangular number */
    if (n == last) {
        return (seq_t)n;              /* unchanged */
    } else {
        return (seq_t)n - 1;          /* shift down by 1 */
    }
}
