/*
 * A383354  Squares of plane partition numbers.
 * https://oeis.org/A383354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000219(n)^2 (square of the plane‑partition number)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for plane partition numbers a(n) */
static seq_t *pp_arr   = NULL;
static size_t pp_cap   = 0;
static idx_t  pp_max   = 0;      /* highest n for which a(n) is known */

/* Static cache for the sums of squared divisors
   s[n] = sum_{d|n} d^2
*/
static seq_t *ss_arr   = NULL;
static size_t ss_cap   = 0;
static idx_t  ss_max   = 0;

/* Ensure the array @arr has capacity at least @target+1.
   Fill any newly allocated entries with @zero_val. */
static void
ensure_capacity(idx_t target, seq_t **arr, size_t *cap, seq_t zero_val)
{
    if (*cap < (size_t)(target + 1)) {
        size_t new_cap = (size_t)(target + 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        for (size_t i = *cap; i < new_cap; ++i)
            (*arr)[i] = zero_val;
        *cap = new_cap;
    }
}

/* Return the n-th term of A383354: a(n) = [A000219(n)]^2   */
seq_t a(idx_t n)
{
    /* ---------- initialise cache for sum of squared divisors ---------- */
    if (ss_arr == NULL) {
        /* delay allocation until needed; this call will also zero any
           newly allocated slots */
        ensure_capacity(n, &ss_arr, &ss_cap, (seq_t)0);
        ss_max = 0;
    }
    if (ss_max < n) {
        idx_t old_ss_max = ss_max;
        ensure_capacity(n, &ss_arr, &ss_cap, (seq_t)0);
        for (idx_t i = old_ss_max + 1; i <= n; ++i) {
            seq_t val = 0;
            for (idx_t d = 1; d * d <= i; ++d) {
                if (i % d == 0) {
                    seq_t d_sq = (seq_t)d * (seq_t)d;
                    val += d_sq;
                    if (d != i / d) {
                        seq_t e = i / d;
                        val += e * e;
                    }
                }
            }
            ss_arr[i] = val;
        }
        ss_max = n;
    }

    /* ---------- initialise cache for plane partition numbers ---------- */
    if (pp_arr == NULL) {
        /* allocate nothing yet; will be done before we use pp_arr[0] */
        ensure_capacity(n, &pp_arr, &pp_cap, (seq_t)0);
        pp_arr[0] = (seq_t)1;     /* a(0) = 1 */
        pp_max   = 0;
    }

    if (pp_max < n) {
        idx_t old_pp_max = pp_max;
        ensure_capacity(n, &pp_arr, &pp_cap, (seq_t)0);
        for (idx_t k = old_pp_max + 1; k <= n; ++k) {
            seq_t sum = 0;
            for (idx_t i = 1; i <= k; ++i)
                sum += ss_arr[i] * pp_arr[k - i];
            pp_arr[k] = sum / (seq_t)k;   /* the division is exact */
        }
        pp_max = n;
    }

    /* ---------- square the plane partition number ---------- */
    seq_t val = pp_arr[n];
    return val * val;
}
