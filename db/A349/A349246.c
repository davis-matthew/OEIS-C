/*
 * A349246  Number of ways to write n as w^8 + x^4 + 2*y^4 + 4*z^4 + t*(t+1),
 *          where w, x, y, z, and t are nonnegative integers.
 * https://oeis.org/A349246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of solutions to n = w^8 + x^4 + 2*y^4 + 4*z^4 + t*(t+1)
 *            with w,x,y,z,t ≥ 0. The function returns a(n)
 * Range:     u8 n <= 0, u16 n <= 7293, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for 64‑bit unsigned values. */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n <= 1) return (idx_t)n;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t cnt = 0;

    for (idx_t w = 0;; ++w) {
        seq_t w2 = (seq_t)w * (seq_t)w;
        seq_t w4 = w2 * w2;
        seq_t w8 = w4 * w4;
        if (w8 > n) break;

        for (idx_t x = 0;; ++x) {
            seq_t x2 = (seq_t)x * (seq_t)x;
            seq_t x4 = x2 * x2;
            seq_t sum_wx = w8 + x4;
            if (sum_wx > n) break;

            for (idx_t y = 0;; ++y) {
                seq_t y2 = (seq_t)y * (seq_t)y;
                seq_t y4 = y2 * y2;
                seq_t sum_wxy = sum_wx + (seq_t)2 * y4;
                if (sum_wxy > n) break;

                for (idx_t z = 0;; ++z) {
                    seq_t z2 = (seq_t)z * (seq_t)z;
                    seq_t z4 = z2 * z2;
                    seq_t sum = sum_wxy + (seq_t)4 * z4;
                    if (sum > n) break;

                    seq_t r = (seq_t)(n - (idx_t)sum);   /* remaining part for t*(t+1) */
                    seq_t D = (seq_t)1 + (seq_t)4 * r;   /* discriminant 1 + 4r */
                    idx_t sqrtD = isqrt_u64((idx_t)D);
                    if ((seq_t)sqrtD * (seq_t)sqrtD == D && (sqrtD & 1)) {
                        ++cnt;                         /* unique t exists */
                    }
                }
            }
        }
    }

    return cnt;
}
