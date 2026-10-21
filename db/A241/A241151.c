/*
 * A241151  Number of distinct degrees in the partition graph G(n) defined at
 *          A241150.
 * https://oeis.org/A241151
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 + (⌊(√(8n + 1) - 1) / 2⌋ - 1) + (⌊(√(8(n – 1) + 1) - 1)
 *            / 2⌋ - 1), for n ≥ 2 (indices below the offset are ignored). The
 *            two terms count the triangular numbers ≤ n and the triangular
 *            numbers + 1 ≤ n, thereby giving the number of distinct degrees
 *            in the partition graph
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t isqrt_uint64(uint64_t x)
{
    /* Integer square root: largest r with r*r ≤ x.  Uses binary search with
       128‑bit multiplication to avoid overflow.  */
    uint64_t low = 0, high = x, r = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= ( __uint128_t)x) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < 2)   /* behaviour for indices < 2 is irrelevant */
        return 0;

    /* Compute the first triangular‐count term */
    uint64_t t1 = 0;
    {
        uint64_t v = 8 * n + 1;
        uint64_t s = isqrt_uint64(v);
        t1 = (s - 1) >> 1;        /* floor((√(8n+1)-1)/2) */
    }
    uint64_t c1 = (t1 > 0) ? t1 - 1 : 0;   /* subtract 1 to exclude m = 1 */

    /* Compute the second triangular‑plus‑1 term */
    uint64_t c2 = 0;
    if (n > 1) {
        uint64_t t2 = 0;
        {
            uint64_t v = 8 * (n - 1) + 1;
            uint64_t s = isqrt_uint64(v);
            t2 = (s - 1) >> 1;    /* floor((√(8(n-1)+1)-1)/2) */
        }
        c2 = (t2 > 0) ? t2 - 1 : 0;   /* also exclude m = 1 */
    }

    return (seq_t)(1 + c1 + c2);
}
