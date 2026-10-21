/*
 * A247988  Least number k such that e - k/(k!)^(1/k) < 1/n.
 * https://oeis.org/A247988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k with e - k/(k!^(1/k)) < 1/n
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state */
    static long double *w          = NULL;   /* w[k-1] = w(k) */
    static size_t  w_cap           = 0;      /* capacity of w */
    static size_t  w_len           = 0;      /* number of entries in w */
    static idx_t   next_k          = 1;      /* next k to be computed */
    static long double total_log   = 0.0L;   /* sum of log(i) for i=1..k */
    static long double e_val       = expl(1.0L);
    static idx_t   max_needed_n    = 1;      /* maximum n requested so far */

    /* keep track of the largest n requested */
    if (n > max_needed_n)
        max_needed_n = n;

    /* extend precomputed w until it's safe to answer all known n */
    while (1) {
        long double w_val;
        /* compute w at next_k */
        total_log += logl((long double)next_k);          /* log(1)=0 */
        long double power = expl(total_log / (long double)next_k);
        w_val = e_val - (long double)next_k / power;

        /* grow array if necessary */
        if (w_len == w_cap) {
            size_t new_cap = (w_cap == 0 ? 256 : w_cap * 2);
            long double *tmp = (long double *)realloc(w, new_cap * sizeof(long double));
            if (!tmp) return 0;  /* out‑of‑memory: return 0 as fail value */
            w = tmp;
            w_cap = new_cap;
        }
        w[w_len++] = w_val;

        /* stop when w(k) < 1/max_needed_n; we may need this for future queries */
        if (w_val < 1.0L / (long double)max_needed_n) {
            next_k++;          /* ready for next computation */
            break;
        }
        next_k++;
    }

    /* binary search for minimal k with w(k) < 1/n */
    size_t left = 0, right = w_len - 1;
    long double target = 1.0L / (long double)n;
    while (left < right) {
        size_t mid = (left + right) >> 1;
        if (w[mid] < target)
            right = mid;
        else
            left = mid + 1;
    }
    idx_t k = (idx_t)(left + 1);  /* array index 0 corresponds to k=1 */
    return (seq_t)k;
}
