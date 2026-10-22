/*
 * A307413  G.f. A(x) satisfies: A(x) = 1 + x*A(x)/(1 - x*A(x) -
 *          2*x^2*A(x)^2).
 * https://oeis.org/A307413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n ≥ 1, the sequence satisfies the functional
 *            equation A(x) = 1 + x*A(x)/(1 - x*A(x) - 2*x^2*A(x)^2). From
 *            this we derive the recurrence a(n) = c_{n-1} - 2*c_{n-2} +
 *            2*d_{n-2}, where c_k = Σ_{i=0}^k a_i a_{k-i}, d_k = Σ_{i=0}^k
 *            a_i c_{k-i}. The implementation stores the already‑compu...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define NMAX 20000   /* maximum n expected (≈10000) */

static seq_t seq_vals[NMAX];
static seq_t c_arr[NMAX];
static seq_t d_arr[NMAX];
static idx_t max_computed = 0xFFFFFFFF;   /* not yet initialized */

seq_t a(idx_t n) {
    /* --- Initialization ---------------------------------------------- */
    if (max_computed == 0xFFFFFFFF) {   /* first call */
        seq_vals[0] = 1;
        c_arr[0]     = seq_vals[0] * seq_vals[0];
        d_arr[0]     = seq_vals[0] * c_arr[0];
        max_computed = 0;
    }

    if (n < max_computed)         /* already computed */
        return seq_vals[n];

    /* --- Compute missing terms -------------------------------------- */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        if (k == 1) {                     /* special step for a(1) */
            seq_vals[1] = 1;
            /* c[1] = a0*a1 + a1*a0 */
            seq_t sum = 0;
            sum += seq_vals[0] * seq_vals[1];
            sum += seq_vals[1] * seq_vals[0];
            c_arr[1] = sum;
            /* d[1] = a0*c1 + a1*c0 */
            sum = 0;
            sum += seq_vals[0] * c_arr[1];
            sum += seq_vals[1] * c_arr[0];
            d_arr[1] = sum;
        } else {                         /* k >= 2 */
            /* compute c_{k-1} = Σ_{i=0}^{k-1} a_i * a_{k-1-i} */
            seq_t sum_c = 0;
            for (idx_t i = 0; i <= k - 1; ++i) {
                sum_c += seq_vals[i] * seq_vals[(k - 1) - i];
            }
            c_arr[k - 1] = sum_c;

            /* compute d_{k-2} = Σ_{i=0}^{k-2} a_i * c_{k-2-i} */
            seq_t sum_d = 0;
            for (idx_t i = 0; i <= k - 2; ++i) {
                sum_d += seq_vals[i] * c_arr[(k - 2) - i];
            }
            d_arr[k - 2] = sum_d;

            /* a_k = c_{k-1} - 2*c_{k-2} + 2*d_{k-2} */
            seq_t t1 = c_arr[k - 1];
            seq_t t2 = c_arr[k - 2];
            seq_t t3 = d_arr[k - 2];
            seq_vals[k] = t1 - (t2 + t2) + (t3 + t3);
        }
    }
    max_computed = n;
    return seq_vals[n];
}
