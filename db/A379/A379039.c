/*
 * A379039  G.f. A(x) satisfies A(x) = ( (1 + x) * (1 + x*A(x)^2) )^2.
 * https://oeis.org/A379039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*k+2,k) *
 *            binomial(4*k+2,n-k)/(2*k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   compute_w_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/compute_w_div.h"

static idx_t max_computed_n = (idx_t)-1;          /* last index for which a(n) is known */
static size_t array_size = 0;                     /* current size of allocated arrays */
static seq_t *a_vals = NULL;                      /* a(n) values */
static seq_t *w_div  = NULL;                      /* binomial(4*k+2,k)/(2*k+1) */
static seq_t *b_term = NULL;                      /* current binomial(4*k+2, m) values */
static size_t max_k_computed = 0;                 /* largest k for which w_div is known */

static void ensure_arrays(idx_t need) {
    size_t new_size = (size_t)need + 1;
    if (new_size <= array_size) return;
    if (a_vals == NULL) {
        a_vals = (seq_t*)malloc(new_size * sizeof(seq_t));
        w_div  = (seq_t*)malloc(new_size * sizeof(seq_t));
        b_term = (seq_t*)malloc(new_size * sizeof(seq_t));
        array_size = new_size;
        /* initialise new elements to 0 */
        for (size_t i = 0; i < new_size; ++i) {
            a_vals[i] = 0;
            w_div[i]  = 0;
            b_term[i] = 0;
        }
    } else {
        a_vals = (seq_t*)realloc(a_vals, new_size * sizeof(seq_t));
        w_div  = (seq_t*)realloc(w_div,  new_size * sizeof(seq_t));
        b_term = (seq_t*)realloc(b_term, new_size * sizeof(seq_t));
        /* initialise new elements to 0 */
        for (size_t i = array_size; i < new_size; ++i) {
            a_vals[i] = 0;
            w_div[i]  = 0;
            b_term[i] = 0;
        }
        array_size = new_size;
    }
}

seq_t a(idx_t n) {
    /* initialize storage if needed */
    if (array_size == 0) {
        ensure_arrays(n);
        /* initialise k = 0 */
        w_div[0]   = (seq_t)1;
        b_term[0]  = (seq_t)1;      /* binomial(2,0) = 1 */
        a_vals[0]  = w_div[0] * b_term[0]; /* a(0) = 1 */
        /* prepare b_term[0] for next index */
        /* binomial(2,1) = 2 */
        b_term[0]  = b_term[0] * (seq_t)(2 - 0) / (seq_t)(1);
        max_computed_n = 0;
        max_k_computed = 0;
    }

    if (n > max_computed_n) {
        /* grow arrays if necessary */
        ensure_arrays(n);

        /* compute w_div for new k values */
        for (idx_t k = max_k_computed + 1; k <= n; ++k) {
            w_div[k]   = compute_w_div(k);
            b_term[k]  = (seq_t)1;     /* binomial(4k+2,0) */
        }
        max_k_computed = n;

        /* extend a[n] values from max_computed_n+1 up to n */
        for (idx_t s = max_computed_n + 1; s <= n; ++s) {
            seq_t sum = 0;
            for (idx_t k = 0; k <= s; ++k) {
                sum += w_div[k] * b_term[k];
                /* update b_term[k] for the next index */
                idx_t m = s - k;
                idx_t t = 4*k + 2;
                if (m < t) {
                    seq_t tmp = b_term[k] * (seq_t)(t - m);
                    b_term[k] = tmp / ((m) + 1);
                } else {
                    b_term[k] = (seq_t)0;
                }
            }
            a_vals[s] = sum;
        }
        max_computed_n = n;
    }

    return a_vals[n];
}
