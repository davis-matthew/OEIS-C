/*
 * A303001  a(n) = permanent of the n X n matrix with entries a(i, i) =
 *          i*(i+1)/2 and a(i, j) = 1 elsewhere.
 * https://oeis.org/A303001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permanent of n×n matrix B with
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* B(i,i) = i*(i+1)/2,  B(i,j) = 1 for i≠j.  */

/* The permanent equals Σ_{k=0}^n e_k(d_1,…,d_n)·D(n–k),  */
/* where e_k are elementary symmetric sums of the diagonal
   entries d_i = i*(i+1)/2 and D(m) are derangements. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* undefined for n=0, but return 0 */

    /* Static state: caches up to the largest n seen so far. */
    static seq_t *a_cache = NULL;   /* permanent values a(1..max_n)   */
    static seq_t *e_vals  = NULL;   /* elementary symmetric sums up to max_n */
    static seq_t *derark  = NULL;   /* derangements D(0..max_n)        */
    static idx_t  max_n   = 0;
    static size_t a_cap   = 0;
    static size_t e_cap   = 0;
    static size_t d_cap   = 0;

    /* Expand caches if needed. */
    if (max_n < n) {
        /* Resize permanent cache. */
        if (a_cache == NULL) {
            ensure_capacity(n + 1, &a_cap, (void**)&a_cache, sizeof(seq_t));
        } else if (a_cap < n + 1) {
            ensure_capacity(n + 1, &a_cap, (void**)&a_cache, sizeof(seq_t));
        }
        /* Resize derangement array. */
        if (derark == NULL) {
            ensure_capacity(n + 1, &d_cap, (void**)&derark, sizeof(seq_t));
            derark[0] = 1;          /* D(0) = 1 */
            derark[1] = 0;          /* D(1) = 0 */
        } else if (d_cap < n + 1) {
            ensure_capacity(n + 1, &d_cap, (void**)&derark, sizeof(seq_t));
        }
        /* Resize elementary sum array. */
        if (e_vals == NULL) {
            ensure_capacity(n + 1, &e_cap, (void**)&e_vals, sizeof(seq_t));
            e_vals[0] = 1;         /* e_0 = 1 */
        } else if (e_cap < n + 1) {
            ensure_capacity(n + 1, &e_cap, (void**)&e_vals, sizeof(seq_t));
        }

        /* Compute new derangements. */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            derark[i] = (i - 1) * (derark[i - 1] + derark[i - 2]);
        }

        /* Incrementally update elementary symmetric sums e_k. */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t di = (seq_t)i * ((seq_t)i + 1) / 2;  /* d_i = i(i+1)/2 */
            /* Update from high to low to avoid overwriting needed values. */
            for (idx_t k = i; k >= 1; --k) {
                e_vals[k] += e_vals[k - 1] * di;
            }
        }

        /* Compute permanent for the new n. */
        seq_t sum = 0;
        for (idx_t k = 0; k <= n; ++k) {
            sum += e_vals[k] * derark[n - k];
        }
        a_cache[n] = sum;

        max_n = n;   /* Update the maximum processed n. */
    }

    /* Result is already cached. */
    return a_cache[n];
}
