/*
 * A364620  G.f. satisfies A(x) = 1/(1-x)^2 + x*A(x)^3.
 * https://oeis.org/A364620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   allocate_seq_t_array, free_seq_t_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A364620
   G.f. satisfies A(x) = 1/(1-x)^2 + x*A(x)^3.
   a(n) = Sum_{k=0..n} binomial(n+3*k+1,4*k+1) *
          binomial(3*k,k) / (2*k+1).
*/
#include <stddef.h>
#include <stdlib.h>
#include "../utils/allocate_seq_t_array.h"
#include "../utils/free_seq_t_array.h"

static void grow_seq_t_array(seq_t **arr, size_t *cap, size_t new_cap) {
    if (new_cap <= *cap) return;
    seq_t *new_arr = allocate_seq_t_array(new_cap);
    for (size_t i = 0; i < *cap; ++i) new_arr[i] = (*arr)[i];
    free_seq_t_array(*arr);
    *arr   = new_arr;
    *cap   = new_cap;
}

/* --- the requested function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static memoised data */
    static seq_t *a_arr = NULL;     /* a[0..max_n]          */
    static seq_t *conv2 = NULL;    /* conv2[0..2*max_n]    */
    static size_t a_cap   = 0;
    static size_t conv_cap= 0;
    static idx_t  cur_max = (idx_t)-1;   /* highest n computed   */

    /* initialise on first call */
    if (cur_max == (idx_t)-1) {
        a_cap   = 1;
        conv_cap= 2 * 0 + 1;          /* size 1 initially   */
        a_arr   = allocate_seq_t_array(a_cap);
        conv2   = allocate_seq_t_array(conv_cap);
        a_arr[0]   = (seq_t)1;        /* a(0) = 1          */
        conv2[0]   = (seq_t)1;        /* a0*a0             */
        cur_max   = 0;
    }

    /* enlarge the arrays if the requested n is larger than we have cached */
    if (n > cur_max) {
        size_t needed_a   = n + 1;
        size_t needed_conv= 2 * n + 1;
        if (needed_a   > a_cap)  grow_seq_t_array(&a_arr,   &a_cap,   needed_a);
        if (needed_conv> conv_cap)grow_seq_t_array(&conv2, &conv_cap, needed_conv);

        /* compute the new terms */
        for (idx_t m = cur_max + 1; m <= n; ++m) {
            seq_t sum = 0;
            for (idx_t i = 0; i < m; ++i) {
                sum += a_arr[i] * conv2[(m - 1) - i];
            }
            a_arr[m] = sum + ((seq_t)m + (seq_t)1);

            /* update the convolution table conv2 for pairs involving a[m] */
            for (idx_t t = 0; t < m; ++t) {
                idx_t idx = m + t;
                conv2[idx] += a_arr[m] * a_arr[t] * 2;
            }
            conv2[2 * m] += a_arr[m] * a_arr[m];
        }
        cur_max = n;
    }

    return a_arr[n];
}
