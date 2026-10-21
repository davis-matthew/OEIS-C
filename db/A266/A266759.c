/*
 * A266759  Growth series for affine Coxeter group (or affine Weyl group) D_4.
 * https://oeis.org/A266759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence for n >= 11 a(n) = 2 a(n‑1)
 *            − a(n‑2) + a(n‑3) − 2 a(n‑4) + 2 a(n‑5) − 2 a(n‑6) + a(n‑7) −
 *            a(n‑8) + 2 a(n‑9) − a(n‑10) with initial values a(0)=1, a(1)=5,
 *            a(2)=14, a(3)=32, a(4)=63, a(5)=110, a(6)=179, a(7)=274,
 *            a(8)=398, a(9)=557, a(10)=754
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 499, u64 n <= 499,
 *            u128 n <= 499, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* coefficients from the rational generating function */
    static const int p[11] = {1, 3, 5, 8, 10, 10, 10, 8, 5, 3, 1};
    static const int q[11] = {1, -2, 1, -1, 2, -2, 2, -1, 1, -2, 1};
    static const int c[10] = {2, -1, 1, -2, 2, -2, 1, -1, 2, -1};

    /* cached initial values a(0) .. a(10) */
    static seq_t init[11];
    static bool ready = false;

    if (!ready) {
        for (idx_t i = 0; i <= 10; ++i) {
            seq_t sum = (seq_t)p[i];
            for (idx_t j = 1; j <= i; ++j) {
                if (q[j] >= 0) {
                    sum -= (seq_t)q[j] * init[i - j];
                } else {
                    sum += (seq_t)(-q[j]) * init[i - j];
                }
            }
            init[i] = sum;
        }
        ready = true;
    }

    if (n <= 10) {
        return init[n];
    }

    /* compute a(n) for n >= 11 using the recurrence */
    seq_t prev[10];
    for (idx_t i = 0; i < 10; ++i) {
        prev[i] = init[10 - i];                 /* prev[0]=a(n-1), prev[9]=a(n-10) */
    }

    for (idx_t k = 11; k <= n; ++k) {
        seq_t new_val = 0;
        for (idx_t j = 0; j < 10; ++j) {
            if (c[j] >= 0) {
                new_val += (seq_t)c[j] * prev[j];
            } else {
                new_val -= (seq_t)(-c[j]) * prev[j];
            }
        }
        /* shift the window */
        for (idx_t j = 9; j > 0; --j) {
            prev[j] = prev[j - 1];
        }
        prev[0] = new_val;
        if (k == n) {
            return new_val;
        }
    }

    /* unreachable, but return 0 for completeness */
    return 0;
}
