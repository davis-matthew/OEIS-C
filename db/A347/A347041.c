/*
 * A347041  Stirling transform of pi (A000720).
 * https://oeis.org/A347041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Stirling2(n,k) * pi(k) (OEIS A347041)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 42,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/compute_pi.h"

static seq_t *S        = NULL; /* current Stirling S(n,k) row, index by k */
static seq_t *a_vals   = NULL; /* stored a(n) values */
static seq_t *pi_arr   = NULL; /* stored pi(k) values */
static idx_t  cap      = 0;    /* array capacity (number of indices stored) */
static idx_t  max_n    = 0;    /* highest n computed so far */

/* Allocate an array of seq_t, value‑initialized to zero */
static seq_t *alloc_seq(idx_t sz)
{
#ifdef __cplusplus
    return new seq_t[sz]();                 /* value‑initialized (zero) */
#else
    return (seq_t *)calloc(sz, sizeof(seq_t));
#endif
}

/* Ensure that the global arrays can hold indices up to 'need' (inclusive) */
static void ensure_capacity(idx_t need)
{
    if (need < cap)
        return;

    idx_t new_cap = need + 1; /* store indices 0..need */
    seq_t *S_new   = alloc_seq(new_cap);
    seq_t *a_new   = alloc_seq(new_cap);
    seq_t *p_new   = alloc_seq(new_cap);

    /* copy old data */
    if (S) {
        for (idx_t i = 0; i < cap; ++i) {
            S_new[i] = S[i];
            a_new[i] = a_vals[i];
            p_new[i] = pi_arr[i];
        }
        /* free old arrays */
#ifdef __cplusplus
        delete[] S;
        delete[] a_vals;
        delete[] pi_arr;
#else
        free(S);
        free(a_vals);
        free(pi_arr);
#endif
    }

    S      = S_new;
    a_vals = a_new;
    pi_arr = p_new;
    cap    = new_cap;
}

/* Public function implementing OEIS sequence A347041 */
seq_t a(idx_t n)
{
    /* First‑time initialization */
    if (S == NULL) {
        ensure_capacity(0);
        /* base case S(0,0) = 1 */
        S[0]        = (seq_t)1;
        a_vals[0]   = (seq_t)0;
        pi_arr[0]   = compute_pi(0); /* pi(0) = 0 */
        max_n       = 0;
    }

    /* Ensure arrays are large enough */
    ensure_capacity(n);

    /* Compute all values up to n (if needed) */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* Compute pi(i) once and store */
            pi_arr[i] = compute_pi(i);

            /* Initialise the new entry for S[i] (new column) */
            S[i] = (seq_t)0;

            /* Update Stirling numbers for row i:
               keep descending to preserve previous row values
               S(k) = k * S(k) + S(k-1)   (k>=1)               */
            for (idx_t k = i; k > 0; --k) {
                S[k] = (seq_t)k * S[k] + S[k - 1];
            }
            /* After the update, S[0] should be 0 for the next row */
            S[0] = (seq_t)0;

            /* Compute a(i) = Σ_{k=1..i} S(k)*pi(k)   (pi(0)=0) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += S[k] * pi_arr[k];
            }
            a_vals[i] = sum;
        }
        max_n = n;
    }

    return a_vals[n];
}
