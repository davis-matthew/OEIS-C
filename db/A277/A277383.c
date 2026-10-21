/*
 * A277383  Each even integer k is followed by k odd integers.
 * https://oeis.org/A277383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   let m = floor(sqrt(n-1)). If n = 1+m², then a(n) = 2m; otherwise
 *            a(n) = 2n – 2m – 3. (OEIS A277383)
 * Range:     u8 n <= 140, u16 n <= 7162, u32 n <= 7162, u64 n <= 7162,
 *            u128 n <= 7162, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned values.  */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = 0xFFFFFFFFULL;            /* maximum sqrt for 64‑bit input */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq  = mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* safety guard; n≥1 in use */
    idx_t m = isqrt_u64(n - 1);         /* m = floor(sqrt(n-1)) */
    if (m * m == n - 1)                 /* n = 1 + m² → even term */
        return (seq_t)(2 * m);
    /* odd term */
    return (seq_t)2 * (seq_t)n
         - (seq_t)2 * (seq_t)m
         - (seq_t)3;
}
