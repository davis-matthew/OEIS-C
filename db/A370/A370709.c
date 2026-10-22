/*
 * A370709  a(n) = 2^n * [x^n] Product_{k>=1} (1 + 2*x^k)^(1/2).
 * https://oeis.org/A370709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^n * [x^n] Product_{k>=1} (1 + 2*x^k)^(1/2)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   compute_c_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compute_c_k.h"

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
        c_arr = (seq_t*)realloc(c_arr, new_size * sizeof(seq_t));
        for (size_t k = c_size; k <= n; ++k)
            c_arr[k] = compute_c_k(k);
        c_size = new_size;
    }

    /* Compute a[n] up to desired n */
    if (a_size <= n) {
        size_t new_size = n + 1;
        a_arr = (seq_t*)realloc(a_arr, new_size * sizeof(seq_t));
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
