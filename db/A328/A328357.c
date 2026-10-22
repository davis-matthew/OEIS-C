/*
 * A328357  Number of inversion sequences of length n avoiding the consecutive
 *          patterns 000, 001, 011, 012.
 * https://oeis.org/A328357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A328357 – Number of inversion sequences of length n avoiding
   the consecutive patterns 000, 001, 011, 012.
   Computed by dynamic programming. */

/*  The DP keeps two arrays:
        inc_arr[i] – number of admissible sequences of current length
                     whose last value is i and whose last step was
                     non‑decreasing.
        dec_arr[i] – same but whose last step was strictly decreasing.
   These are updated in O(n²) time, while the full sequence a(n)
   is accumulated in a_vals.  All arithmetic uses type seq_t only. */

#include <stdlib.h>

#ifdef __cplusplus
#include <cstddef>
#endif

static idx_t max_len = 0;          /* largest n for which a_vals is valid   */
static idx_t cap     = 0;          /* allocated capacity of the arrays      */
static seq_t *inc_arr = NULL;      /* counts for non‑decreasing last step   */
static seq_t *dec_arr = NULL;      /* counts for decreasing last step      */
static seq_t *inc_tmp = NULL;      /* temporary buffer for next iteration  */
static seq_t *dec_tmp = NULL;      /* temporary buffer for next iteration  */
static seq_t *a_vals = NULL;       /* array of a(n) values                  */

/* Grow a seq_t array from old_cap to new_cap.
   In C++ the new array is constructed with copy–assignment. */
static seq_t *grow(seq_t *old, size_t old_cap, size_t new_cap)
{
#ifdef __cplusplus
    seq_t *q = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i) q[i] = old[i];
    delete[] old;
    return q;
#else
    /* In C the type seq_t is a plain POD type, realloc is fine. */
    return (seq_t *)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Ensure that a_vals[0..n] are computed and stored. */
static void precompute_to(idx_t n)
{
    if (max_len >= n) return;          /* already computed                 */

    /* Allocate or grow the arrays if required, at least (n+1) slots. */
    if (cap < n + 1) {
        size_t new_cap = n + 1;
        inc_arr  = grow(inc_arr, cap, new_cap);
        dec_arr  = grow(dec_arr, cap, new_cap);
        inc_tmp  = grow(inc_tmp, cap, new_cap);
        dec_tmp  = grow(dec_tmp, cap, new_cap);
        a_vals   = grow(a_vals,  cap, new_cap);
        cap = new_cap;
    }

    /* Initialization for length 1 (n = 0 in the DP’s zero‑based sense). */
    if (max_len == 0) {
        inc_arr[0] = (seq_t)1;
        dec_arr[0] = (seq_t)1;
        a_vals[0]  = (seq_t)1;            /* a(0) = 1 */
        if (cap > 1) a_vals[1] = (seq_t)1;/* a(1) = 1 */
        max_len = 1;                     /* we know a(0) and a(1)           */
    }

    /* Extend the DP one step at a time up to n. */
    for (idx_t len = max_len; len < n; ++len) {
        /* Compute inc_tmp[i] = sum_{j<=i} dec_arr[j]  (non‑decreasing step) */
        seq_t cum = (seq_t)0;
        for (idx_t i = 0; i < len; ++i) {
            cum += dec_arr[i];
            inc_tmp[i] = cum;
        }
        inc_tmp[len] = cum;               /* inc_tmp[len] = total dec_arr */

        /* Compute dec_tmp[i] = sum_{j>i} (inc_arr[j] + dec_arr[j]) (decreasing) */
        cum = (seq_t)0;
        for (idx_t i = len; i > 0; --i) {
            seq_t v_inc = inc_arr[i - 1];
            seq_t v_dec = dec_arr[i - 1];
            dec_tmp[i - 1] = cum;
            cum += v_inc + v_dec;
        }
        dec_tmp[len] = (seq_t)0;          /* outside valid range */

        /* Swap current and temporary arrays. */
        seq_t *swap;
        swap = inc_arr;  inc_arr  = inc_tmp;  inc_tmp  = swap;
        swap = dec_arr;  dec_arr  = dec_tmp;  dec_tmp  = swap;

        /* Compute a(len+1) by summing all entries of inc_arr and dec_arr. */
        seq_t total = (seq_t)0;
        for (idx_t i = 0; i <= len; ++i)
            total += inc_arr[i] + dec_arr[i];
        a_vals[len + 1] = total;
    }

    max_len = n;                          /* now a(n) is available            */
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */
    /* a(1) = 1 is also handled by precomputation below, but a quick check
       avoids a call when the caller explicitly queries small n. */
    if (n == 1) return (seq_t)1;
    precompute_to(n);
    return a_vals[n];
}
