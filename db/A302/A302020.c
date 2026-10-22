/*
 * A302020  Expansion of 1/(1 - x*Product_{k>=1} (1 + x^(2*k))/(1 -
 *          x^(2*k-1))).
 * https://oeis.org/A302020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A302020 (Expansion of 1/(1 - x*Product_{k≥1} (1 + x^(2k))/(1 - x^(2k-1))) )
   a(0) = 1
   a(n) = Σ_{k=1..n} A001935(k-1)*a(n-k)  where
       A001935(m) = coefficient of ∏_{k≥1}(1 + x^k + x^{2k} + x^{3k})
*/

/* Note: Requires the utility headers provided in the build. */
#include <stddef.h>
#include <string.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

/* ----- internal state ----- */
static seq_t *c_arr = NULL;                /* coefficients of ∏(1+x^k+x^{2k}+x^{3k}) */
static seq_t *a_arr = NULL;                /* values of the sequence */
static size_t  arr_cap = 0;                /* allocated capacity (in elements) */
static idx_t   max_c_computed = 0;         /* highest n for which c_arr has been filled */
static idx_t   max_a_computed = 0;         /* highest n for which a_arr has been filled */

/* ----- compute c_arr[0..N] using bounded partition DP (≤3 copies of each part) ----- */
static void compute_c_upto(idx_t N)
{
    /* allocate temporary DP arrays of length N+1 */
    seq_t *dp_prev = alloc_seq((size_t)N + 1);
    seq_t *dp_new  = alloc_seq((size_t)N + 1);

    /* initialise dp_prev: only sum 0 is possible */
    dp_prev[0] = 1;
    for (idx_t i = 1; i <= N; ++i) dp_prev[i] = 0;

    /* iterate over part sizes */
    for (idx_t part = 1; part <= N; ++part) {
        for (idx_t i = 0; i <= N; ++i) {
            seq_t val = dp_prev[i];
            if (i >= part)          val += dp_prev[i - part];
            if (i >= 2 * part)      val += dp_prev[i - 2 * part];
            if (i >= 3 * part)      val += dp_prev[i - 3 * part];
            dp_new[i] = val;
        }
        /* swap dp_prev and dp_new for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev  = dp_new;
        dp_new   = tmp;
    }

    /* ensure c_arr has enough space */
    if (!c_arr || arr_cap < (size_t)N + 1) {
        if (c_arr) free_seq(c_arr);
        c_arr = alloc_seq((size_t)N + 1);
        arr_cap = (size_t)N + 1;
    }

    /* copy the result into c_arr */
    memcpy(c_arr, dp_prev, ((size_t)N + 1) * sizeof(seq_t));

    /* free temporary arrays */
    free_seq(dp_prev);
    free_seq(dp_new);
}

/* ----- resize internal arrays if necessary ----- */
static void ensure_capacity(idx_t N)
{
    size_t new_cap = (size_t)N + 1;
    if (new_cap <= arr_cap) return;

    seq_t *new_c = alloc_seq(new_cap);
    seq_t *new_a = alloc_seq(new_cap);

    /* copy existing contents if any */
    if (c_arr) memcpy(new_c, c_arr, arr_cap * sizeof(seq_t));
    if (a_arr) memcpy(new_a, a_arr, arr_cap * sizeof(seq_t));

    /* replace old arrays */
    if (c_arr) free_seq(c_arr);
    if (a_arr) free_seq(a_arr);
    c_arr = new_c;
    a_arr = new_a;
    arr_cap = new_cap;

    /* initialise the first element of the sequence */
    a_arr[0] = 1;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    /* make sure we can store values up to n */
    ensure_capacity(n);

    /* compute coefficients of the auxiliary product if needed */
    if (max_c_computed < n) {
        compute_c_upto(n);
        max_c_computed = n;
    }

    /* compute a[n] incrementally from what we already have */
    if (max_a_computed < n) {
        for (idx_t m = max_a_computed + 1; m <= n; ++m) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= m; ++k)
                sum += c_arr[k - 1] * a_arr[m - k];
            a_arr[m] = sum;
        }
        max_a_computed = n;
    }

    return a_arr[n];
}
