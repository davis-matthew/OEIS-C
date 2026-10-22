/*
 * A367076  Irregular triangle read by rows: T(n,k) (0 <= n, 0 <= k < 2^n).
 *          T(n,k) = -Sum_{i=0..k} A365968(n,i).
 * https://oeis.org/A367076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look, tabf
 * Formula:   a(n) = T(floor_log2(n+1), n - (2^{floor_log2(n+1)} - 1)) * where
 *            * T(n,k) = Σ_{i=1..n} | (k+1) - 2^i * round((k+1)/2^i) | * i *
 *            round(x) = ⌊x + ½⌋ for positive x, implemented as * (k+1 +
 *            2^(i-1)) / 2^i
 * Range:     u8 n <= 138, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* row index r = ⌊log2(n+1)⌋ */
    idx_t r = floor_log2_uint64(n + 1);

    /* number of elements before row r */
    idx_t prev = ((idx_t)1 << r) - 1;

    /* column index k within row r */
    idx_t k = n - prev;

    seq_t sum = 0;
    idx_t one = 1;

    for (idx_t i = 1; i <= r; ++i)
    {
        idx_t pow2  = one << i;                    /* 2^i          */
        idx_t kp1   = k + 1;                       /* k+1          */
        idx_t round = (kp1 + (pow2 >> 1)) / pow2;  /* round((k+1)/2^i) */
        idx_t prod  = pow2 * round;                /* 2^i * round   */
        idx_t diff  = (kp1 >= prod) ? (kp1 - prod) : (prod - kp1); /* |k+1 - prod| */

        sum += (seq_t)diff * (seq_t)i;
    }

    return sum;
}
