/*
 * A370709  a(n) = 2^n * [x^n] Product_{k>=1} (1 + 2*x^k)^(1/2).
 * https://oeis.org/A370709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^n * [x^n] Product_{k>=1} (1 + 2*x^k)^(1/2)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper to compute c_k = k*b_k*2^k, where
   b_k = (1/2) * Σ_{d|k} (-1)^{d+1} * 2^d / d  */
static seq_t compute_c_k(idx_t k)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= k; ++d) {
        if (k % d == 0) {
            idx_t d1 = d;
            idx_t d2 = k / d;

            /* divisor d1 */
            seq_t term = ((seq_t)1 << (unsigned long long)(k + d1))
                         * (seq_t)(k / d1);
            term /= 2;
            if (d1 & 1)    /* d1 odd -> sign +1 */
                sum += term;
            else
                sum -= term;

            /* divisor d2 if distinct */
            if (d1 != d2) {
                seq_t term2 = ((seq_t)1 << (unsigned long long)(k + d2))
                              * (seq_t)(k / d2);
                term2 /= 2;
                if (d2 & 1)
                    sum += term2;
                else
                    sum -= term2;
            }
        }
    }
    return sum;
}

/* Main function computing a(n) */
seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *a_arr   = NULL;
    static size_t  a_size = 0;
    static seq_t *c_arr   = NULL;
    static size_t  c_size = 0;

    /* Ensure c[k] are computed up to n */
    if (c_size <= n) {
        size_t new_size = n + 1;
        c_arr = mem_resize(seq_t, c_arr, new_size);
        for (size_t k = c_size; k <= n; ++k)
            c_arr[k] = compute_c_k(k);
        c_size = new_size;
    }

    /* Compute a[n] up to desired n */
    if (a_size <= n) {
        size_t new_size = n + 1;
        a_arr = mem_resize(seq_t, a_arr, new_size);
        for (size_t cur = a_size; cur <= n; ++cur) {
            if (cur == 0) {
                a_arr[0] = 1;
                continue;
            }
            seq_t sum = 0;
            for (size_t k = 1; k <= cur; ++k)
                sum += c_arr[k] * a_arr[cur - k];
            a_arr[cur] = sum / (seq_t)cur;  /* exact division */
        }
        a_size = new_size;
    }

    return a_arr[n];
}
