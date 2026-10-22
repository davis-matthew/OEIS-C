/*
 * A329668  Number of meanders of length n with Motzkin-steps avoiding the
 *          consecutive steps HH and DU.
 * https://oeis.org/A329668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   Computes the number of meanders of length n that use Motzkin
 *            steps * (U=(1,1), H=(1,0), D=(1,-1)) with the forbidden
 *            consecutive steps * HH and DU. The path must start at altitude 0
 *            and never go below it. * The function uses dynamic programming
 *            that tracks the current altitude * and the last step...
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache       = NULL;     // a_cache[n] = a(n)
static idx_t  cache_size    = 0;        // allocated size of a_cache
static size_t array_size    = 0;        // allocated size for the DP arrays
static seq_t *u_arr         = NULL;     // paths ending in U
static seq_t *h_arr         = NULL;     // paths ending in H
static seq_t *d_arr         = NULL;     // paths ending in D
static seq_t *nu_arr        = NULL;     // U for next step
static seq_t *nh_arr        = NULL;     // H for next step
static seq_t *nd_arr        = NULL;     // D for next step
static idx_t  computed_len  = 0;        // longest n already computed

/* Expand the memoisation tables up to at least n */
static void ensure_up_to(idx_t n)
{
    /* Resize the a_cache array if necessary */
    idx_t need_cache = n + 1;   /* store values for indices 0..n */
    if ((size_t)need_cache > cache_size) {
        seq_t *tmp = (seq_t *)realloc(a_cache, need_cache * sizeof(seq_t));
        a_cache = tmp;
        cache_size = need_cache;
    }

    /* Resize the DP arrays if necessary (indices 0..n) */
    size_t need_size = (size_t)n + 1;
    if (need_size > array_size) {
        u_arr  = (seq_t *)realloc(u_arr,  need_size * sizeof(seq_t));
        h_arr  = (seq_t *)realloc(h_arr,  need_size * sizeof(seq_t));
        d_arr  = (seq_t *)realloc(d_arr,  need_size * sizeof(seq_t));
        nu_arr = (seq_t *)realloc(nu_arr, need_size * sizeof(seq_t));
        nh_arr = (seq_t *)realloc(nh_arr, need_size * sizeof(seq_t));
        nd_arr = (seq_t *)realloc(nd_arr, need_size * sizeof(seq_t));

        /* Zero the newly allocated portion */
        for (size_t i = array_size; i < need_size; i++) {
            u_arr[i]  = 0;
            h_arr[i]  = 0;
            d_arr[i]  = 0;
            nu_arr[i] = 0;
            nh_arr[i] = 0;
            nd_arr[i] = 0;
        }
        array_size = need_size;
    }

    /* Initialise cache[0] once */
    a_cache[0] = 1;

    /* Compute values up to n */
    while (computed_len < n) {
        idx_t cur_len  = computed_len;
        idx_t next_len = cur_len + 1;

        /* Zero the arrays that will hold the new step */
        for (idx_t i = 0; i <= next_len; i++) {
            nu_arr[i] = 0;
            nh_arr[i] = 0;
            nd_arr[i] = 0;
        }

        if (cur_len == 0) {
            /* First step from the empty path */
            nu_arr[1] = 1;   /* U */
            nh_arr[0] = 1;   /* H */
        } else {
            /* Transition from all states of length cur_len */
            for (idx_t h = 0; h <= cur_len; h++) {
                seq_t up  = u_arr[h];
                seq_t hp  = h_arr[h];
                seq_t dp  = d_arr[h];

                if (up == 0 && hp == 0 && dp == 0)
                    continue; /* skip empty state */

                /* Next step U : altitude increases by 1, prev step not D */
                nu_arr[h + 1] += up + hp;

                /* Next step H : altitude unchanged, prev step not H */
                nh_arr[h] += up + dp;

                /* Next step D : altitude decreases by 1, any prev step */
                if (h > 0) {
                    nd_arr[h - 1] += up + hp + dp;
                }
            }
        }

        /* Sum all paths of length next_len, regardless of final altitude or last step */
        seq_t sum = 0;
        for (idx_t i = 0; i <= next_len; i++) {
            sum += nu_arr[i] + nh_arr[i] + nd_arr[i];
        }
        a_cache[next_len] = sum;

        /* Move the new arrays to the current position for the next iteration */
        for (idx_t i = 0; i <= next_len; i++) {
            u_arr[i] = nu_arr[i];
            h_arr[i] = nh_arr[i];
            d_arr[i] = nd_arr[i];
        }

        computed_len = next_len;
    }
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return a_cache[n];
}
