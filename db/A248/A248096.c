/*
 * A248096  Number of ordered trees with n edges such that non-leaf vertices
 *          at even (odd) level have an odd (even) number of children.
 * https://oeis.org/A248096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of z^n in the generating function g
 *            defined by g = 1 + z*h/(1 - z^2*h^2) and h = 1/(1 - z^2*g^2).
 *            Computation uses an O(n^2) recurrence
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static idx_t cap = 0;
    static seq_t *a_arr = NULL, *h_arr = NULL, *f_arr = NULL;
    static seq_t *g_arr = NULL, *q_arr = NULL;

    if (n >= cap) {
        idx_t old_cap = cap;
        idx_t new_cap = n + 1;
        if (cap == 0 || new_cap <= cap) {
            new_cap = (cap == 0) ? 1 : (cap << 1);
        }
        if (new_cap < n + 1) new_cap = n + 1;

        /* Allocate new arrays */
        seq_t *a_new = (seq_t*)malloc(new_cap * sizeof(seq_t));
        seq_t *h_new = (seq_t*)malloc(new_cap * sizeof(seq_t));
        seq_t *f_new = (seq_t*)malloc(new_cap * sizeof(seq_t));
        seq_t *g_new = (seq_t*)malloc(new_cap * sizeof(seq_t));
        seq_t *q_new = (seq_t*)malloc(new_cap * sizeof(seq_t));

        /* zero initialise new entries */
        for (idx_t i = 0; i < new_cap; ++i) {
            a_new[i] = 0; h_new[i] = 0; f_new[i] = 0; g_new[i] = 0; q_new[i] = 0;
        }

        /* Copy old data */
        if (old_cap) {
            for (idx_t i = 0; i < old_cap; ++i) {
                a_new[i] = a_arr[i];
                h_new[i] = h_arr[i];
                f_new[i] = f_arr[i];
                g_new[i] = g_arr[i];
                q_new[i] = q_arr[i];
            }
        } else {   /* base initialization */
            a_new[0] = 1;          /* a(0) */
            h_new[0] = 1;          /* h(0) = 1 */
            f_new[0] = 1;          /* f(0) = a0*a0 */
            g_new[0] = 1;          /* g(0) = h0*h0 */
            q_new[0] = 1;          /* q(0) = 1 */
        }

        /* release old arrays */
        free(a_arr); free(h_arr); free(f_arr); free(g_arr); free(q_arr);

        /* install new arrays */
        a_arr = a_new; h_arr = h_new; f_arr = f_new; g_arr = g_new; q_arr = q_new;
        cap   = new_cap;

        /* compute new terms from old_cap up to n */
        for (idx_t m = old_cap; m <= n; ++m) {
            if (m == 0) continue;  /* base already set */

            /* ---- compute h[m] ---- */
            if (m == 1) {
                h_arr[m] = 0;
            } else {
                seq_t sum = 0;
                for (idx_t k = 0; k <= m - 2; ++k) {
                    sum += f_arr[k] * h_arr[m - 2 - k];
                }
                h_arr[m] = sum;
            }

            /* ---- compute q[m] ---- */
            if (m == 1) {
                q_arr[m] = 0;
            } else {
                seq_t sum = 0;
                for (idx_t k = 0; k <= m - 2; ++k) {
                    sum += g_arr[k] * q_arr[m - 2 - k];
                }
                q_arr[m] = sum;
            }

            /* ---- compute a[m] ---- */
            seq_t sum = 0;
            for (idx_t i = 0; i <= m - 1; ++i) {
                sum += h_arr[i] * q_arr[m - 1 - i];
            }
            a_arr[m] = sum;

            /* ---- update f[m] = convolution of a with itself ---- */
            seq_t fsum = 0;
            for (idx_t i = 0; i <= m; ++i) {
                fsum += a_arr[i] * a_arr[m - i];
            }
            f_arr[m] = fsum;

            /* ---- update g[m] = convolution of h with itself ---- */
            seq_t gsum = 0;
            for (idx_t i = 0; i <= m; ++i) {
                gsum += h_arr[i] * h_arr[m - i];
            }
            g_arr[m] = gsum;
        }
    }

    return a_arr[n];
}
