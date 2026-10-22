/*
 * A398480  Triangle read by rows: T(n, k) = Sum_{i=0..n-k} RisingFactorial(i
 *          + 1, n - i - k) * n^(n - i).
 * https://oeis.org/A398480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8 n+1) – 1)/2⌋, k = n – r(r + 1)/2,
 *            and T(r,k) = Σ_{i=0}^{m} (m! / i!) · r^{\,r‑i} with m = r – k
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 27, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square‑root of a 64‑bit integer (floor of √n). */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Get (row, column) from linear index n. */
    idx_t t = isqrt_u64(8 * n + 1);
    idx_t r = (t - 1) >> 1;               /* floor((√(8n+1)-1)/2) */
    idx_t before = r * (r + 1) / 2;        /* number of terms before row r */
    idx_t k = n - before;                 /* column within the row */
    idx_t m = r - k;                      /* upper limit of i in the sum */

    /* Compute rising factorial (m)! */
    seq_t rising = 1;
    for (idx_t i = 2; i <= m; ++i)
        rising *= (seq_t)i;

    /* Compute power r^r */
    seq_t power = 1;
    for (idx_t i = 0; i < r; ++i)
        power *= (seq_t)r;

    /* Sum the terms.  Update rising and power on the fly. */
    seq_t sum = 0;
    for (idx_t i = 0; i <= m; ++i) {
        sum += rising * power;
        if (i != m) {                    /* avoid division by zero when r==0 */
            rising = rising / (seq_t)(i + 1);
            power   = power   / (seq_t)r;
        }
    }
    return sum;
}
