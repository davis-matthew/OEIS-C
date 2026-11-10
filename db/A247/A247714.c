/*
 * A247714  Position of A036561(n) in sequence A003586.
 * https://oeis.org/A247714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{3-smooth numbers <= A036561(n)}; for A036561(n) =
 *            2^i*3^k, a(n) = Sum_{m=0..k} (i + floor(m*log_2(3)) + 1) +
 *            Sum_{t>=1, 3^t<=2^i} (i - floor(t*log_2(3)))
 * Range:     u8 n <= 186, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9738 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A247714_floor_mlog23(idx_t m)
{
    const long double L = 1.58496250072115618145373894394781650875981440769248L;
    return (idx_t)((long double)m * L);
}

seq_t a(idx_t n)
{
    idx_t r = 0;
    while (n >= (r + 1) * (r + 2) / 2) r++;
    idx_t k = n - r * (r + 1) / 2;
    idx_t i = r - k;
    idx_t cnt = 0;
    idx_t m, t;
    /* powers 3^j with j <= k: x/3^j = 2^i * 3^(k-j) */
    for (m = 0; m <= k; m++)
        cnt += i + A247714_floor_mlog23(m) + 1;
    /* powers 3^(k+t), t >= 1, while 3^t <= 2^i */
    for (t = 1;; t++) {
        idx_t f = A247714_floor_mlog23(t);
        if (f >= i) break;           /* 3^t > 2^i */
        cnt += i - f;                /* floor(log2(2^i/3^t)) + 1 */
    }
    return (seq_t)cnt;
}
