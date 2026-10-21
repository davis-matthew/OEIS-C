/*
 * A259577  Sum of numbers in the n-th antidiagonal of the reciprocity array
 *          of 1.
 * https://oeis.org/A259577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..n} Sum_{k=0..m-1} floor((n*k + 1)/m)
 * Range:     u8 n <= 10, u16 n <= 64, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the iterative AtCoder floor_sum algorithm. */

#include <stdint.h>

static inline seq_t floor_sum(idx_t n, idx_t m, idx_t a, idx_t b) {
    seq_t res = 0;
    while (1) {
        if (a >= m) {
            idx_t q = a / m;
            /* res += (n-1) * n * q / 2 */
            __int128 tmp = (__int128)(n - 1) * n;
            tmp = tmp * q;
            tmp /= 2;
            res += (seq_t)tmp;
            a %= m;
        }
        if (b >= m) {
            idx_t q = b / m;
            res += (seq_t)(n * q);
            b %= m;
        }
        idx_t y_max = a * n + b;
        if (y_max < m) break;
        n = y_max / m;
        b = y_max % m;
        /* swap m and a */
        idx_t tmp = m;
        m = a;
        a = tmp;
    }
    return res;
}

seq_t a(idx_t n) {
    seq_t ans = 0;
    for (idx_t m = 1; m <= n; ++m) {
        ans += floor_sum(m, m, n, 1);
    }
    return ans;
}
