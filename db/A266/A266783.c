/*
 * A266783  The growth series for the affine Coxeter (or Weyl) group [3,3,5]
 *          (or H_4).
 * https://oeis.org/A266783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in
 *     (1 - x^2)*(1 - x^12)*(1 - x^20)*(1 - x^30) /
 *     ((1 - x)^5*(1 - x^11)*(1 - x^19)*(1 - x^29))
 *
 * The recurrence follows from the rational generating function
 *  G(x) = P(x) / Q(x)
 * where Q(0) = 1.  Hence for every n ≥ 0:
 *     a(n) = -∑_{k=1..64} Q[k] * a(n-k) + P[n]   (P[n] = 0 if n > 64)
 *
 * The coefficients of P and Q are pre‑computed once.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* coefficient arrays for Q and P, indices 0..64 */
    static int Q[65] = {0};
    static int P[65] = {0};
    static int init = 0;

    if (!init) {
        /* Build Q = (1 - x)^5 * (1 - x^11) * (1 - x^19) * (1 - x^29) */
        int temp[65] = {0};
        /* (1 - x)^5 */
        int A[6] = {1, -5, 10, -10, 5, -1};
        for (int i = 0; i < 6; ++i) temp[i] = A[i];
        /* multiply by (1 - x^11) */
        for (int i = 64; i >= 11; --i) temp[i] -= temp[i-11];
        /* multiply by (1 - x^19) */
        for (int i = 64; i >= 19; --i) temp[i] -= temp[i-19];
        /* multiply by (1 - x^29) */
        for (int i = 64; i >= 29; --i) temp[i] -= temp[i-29];
        memcpy(Q, temp, sizeof(temp));

        /* Build P = (1 - x^2) * (1 - x^12) * (1 - x^20) * (1 - x^30) */
        int tmp[65] = {0};
        tmp[0] = 1;      /* constant term */
        tmp[2] = -1;     /* -x^2 */
        for (int i = 64; i >= 12; --i) tmp[i] -= tmp[i-12];
        for (int i = 64; i >= 20; --i) tmp[i] -= tmp[i-20];
        for (int i = 64; i >= 30; --i) tmp[i] -= tmp[i-30];
        memcpy(P, tmp, sizeof(tmp));

        init = 1;
    }

    static seq_t *buf = NULL;
    static idx_t buf_len = 0;   /* already allocated size */

    /* Ensure buffer has slot 0 set to 1 */
    if (buf_len == 0) {
        buf = (seq_t *)malloc(sizeof(seq_t));
        if (!buf) return 0;   /* allocation failure */
        buf[0] = 1;           /* a(0) = 1 */
        buf_len = 1;
    }

    if (n >= buf_len) {
        idx_t new_len = n + 1;
        seq_t *new_buf = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_buf) return 0;   /* allocation failure */
        if (buf) {
            memcpy(new_buf, buf, buf_len * sizeof(seq_t));
            free(buf);
        }
        buf = new_buf;

        /* compute new entries */
        for (idx_t i = buf_len; i <= n; ++i) {
            seq_t acc = 0;
            /* recurrence from Q */
            for (int k = 1; k <= 64; ++k) {
                if (i >= (idx_t)k)
                    acc -= (seq_t)Q[k] * buf[i - k];
            }
            /* add numerator coefficient if in bounds */
            if (i <= 64)
                acc += (seq_t)P[i];
            buf[i] = acc;
        }
        buf_len = new_len;
    }

    return buf[n];
}
