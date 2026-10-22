/*
 * A375002  a(n) = Sum_{i+j+k+m=n, i,j,k,m >= 1} tau(i) * tau(j) * tau(k) *
 *          tau(m).
 * https://oeis.org/A375002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i+j+k+m=n, i,j,k,m >= 1} tau(i) * tau(j) * tau(k) *
 *            tau(m)
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a two‑step convolution: first compute conv2(n)=∑_{i=1}^{n-1}tau(i)tau(n-i),
   then a(n)=∑_{s=2}^{n-2}conv2(s)conv2(n-s). */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static seq_t *tau_arr = NULL;
static seq_t *conv2_arr = NULL;
static seq_t *a_arr    = NULL;
static idx_t max_n = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined below offset, but guard */
    if (n > max_n) {
        /* Allocate new arrays of size n+1 (index 0 unused). */
        size_t sz = (size_t)n + 1;
        seq_t *tau = (seq_t*)malloc(sz * sizeof(seq_t));
        seq_t *conv2 = (seq_t*)malloc(sz * sizeof(seq_t));
        seq_t *a_val = (seq_t*)malloc(sz * sizeof(seq_t));

        /* Initialise to zero */
        for (size_t i = 0; i < sz; ++i) {
            tau[i] = 0;
            conv2[i] = 0;
            a_val[i] = 0;
        }

        /* Compute divisor function tau(k) via sieve */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i) {
                tau[j] += 1;
            }
        }

        /* First convolution: conv2[s] = sum_{i+j=s} tau[i]tau[j]  (i,j>=1) */
        for (idx_t i = 1; i <= n; ++i) {
            if (tau[i] == 0) continue;
            for (idx_t j = 1; i + j <= n; ++j) {
                conv2[i + j] += tau[i] * tau[j];
            }
        }

        /* Second convolution gives the 4‑fold sum. */
        for (idx_t k = 1; k <= n; ++k) {
            if (k < 4) {               /* fewer than 4 terms, result is 0 */
                a_val[k] = 0;
                continue;
            }
            seq_t sum = 0;
            for (idx_t s = 2; s <= k - 2; ++s) {
                sum += conv2[s] * conv2[k - s];
            }
            a_val[k] = sum;
        }

        /* Replace the old arrays with the new ones. */
        free(tau_arr);
        free(conv2_arr);
        free(a_arr);
        tau_arr = tau;
        conv2_arr = conv2;
        a_arr = a_val;
        max_n = n;
    }

    return a_arr[n];
}
