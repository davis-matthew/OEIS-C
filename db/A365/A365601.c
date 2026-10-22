/*
 * A365601  Expansion of e.g.f. 1 / (1 - 5 * log(1 + x))^(2/5).
 * https://oeis.org/A365601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (Product_{j=0..k-1} (5*j+2)) *
 *            Stirling1(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#if defined(__cplusplus)
#include <algorithm>
#endif

/* Ensure that cache and arrays are large enough for index `need`. */
static void ensure_capacity(idx_t need,
                            seq_t **cache_ptr,   idx_t *cache_size_ptr,
                            seq_t **p_arr_ptr,    idx_t *p_arr_size_ptr,
                            seq_t **st_prev_ptr,  idx_t *st_prev_size_ptr,
                            seq_t **st_curr_ptr,  idx_t *st_curr_size_ptr)
{
    if (*cache_size_ptr <= need) {
        idx_t new_size = need + 1;
        seq_t *new_cache;
        seq_t *new_p_arr;
        seq_t *new_st_prev;
        seq_t *new_st_curr;
#ifdef __cplusplus
        new_cache = new seq_t[new_size];
        new_p_arr = new seq_t[new_size];
        new_st_prev = new seq_t[new_size];
        new_st_curr = new seq_t[new_size];
        for (idx_t i = 0; i < *cache_size_ptr; ++i) {
            new_cache[i] = (*cache_ptr)[i];
            new_p_arr[i] = (*p_arr_ptr)[i];
            new_st_prev[i] = (*st_prev_ptr)[i];
            new_st_curr[i] = (*st_curr_ptr)[i];
        }
        for (idx_t i = *cache_size_ptr; i < new_size; ++i) {
            new_cache[i] = 0;
            new_p_arr[i] = 0;
            new_st_prev[i] = 0;
            new_st_curr[i] = 0;
        }
        if (*cache_ptr != NULL) delete [] *cache_ptr;
        if (*p_arr_ptr != NULL) delete [] *p_arr_ptr;
        if (*st_prev_ptr != NULL) delete [] *st_prev_ptr;
        if (*st_curr_ptr != NULL) delete [] *st_curr_ptr;
#else
        new_cache = (seq_t *)realloc(*cache_ptr,
                                     new_size * sizeof(seq_t));
        new_p_arr = (seq_t *)realloc(*p_arr_ptr,
                                     new_size * sizeof(seq_t));
        new_st_prev = (seq_t *)realloc(*st_prev_ptr,
                                       new_size * sizeof(seq_t));
        new_st_curr = (seq_t *)realloc(*st_curr_ptr,
                                       new_size * sizeof(seq_t));
        if (new_cache == NULL || new_p_arr == NULL ||
            new_st_prev == NULL || new_st_curr == NULL) {
            /* Allocation failed; exit to avoid undefined behavior. */
            abort();
        }
        for (idx_t i = *cache_size_ptr; i < new_size; ++i) {
            new_cache[i] = 0;
            new_p_arr[i] = 0;
            new_st_prev[i] = 0;
            new_st_curr[i] = 0;
        }
#endif
        *cache_ptr = new_cache;
        *p_arr_ptr = new_p_arr;
        *st_prev_ptr = new_st_prev;
        *st_curr_ptr = new_st_curr;
        *cache_size_ptr = new_size;
    }
}

/* The sequence a(n) */
seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache = NULL;
    static seq_t *P_arr = NULL;
    static seq_t *st_prev = NULL;
    static seq_t *st_curr = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = 0;

    /* Allocate initial storage if needed */
    if (cache == NULL) {
        ensure_capacity(0, &cache, &cache_size,
                        &P_arr, &cache_size,
                        &st_prev, &cache_size,
                        &st_curr, &cache_size);
        /* Initialise first term and Stirling row */
        cache[0] = 1;
        P_arr[0] = 1;
        st_prev[0] = 1;
        /* All other entries are zero because of allocate */
        last_computed = 0;
    }

    if (n <= last_computed) {
        /* Already computed */
        return cache[n];
    }

    /* Ensure capacity for all arrays up to n */
    ensure_capacity(n, &cache, &cache_size,
                    &P_arr, &cache_size,
                    &st_prev, &cache_size,
                    &st_curr, &cache_size);

    /* Incrementally compute from last_computed+1 up to n */
    for (idx_t cur = last_computed + 1; cur <= n; ++cur) {
        /* Compute P_k up to current cur */
        for (idx_t k = cur; k <= cur; ++k) {
            /* P[k] = P[k-1] * (5*(k-1)+2) */
            P_arr[k] = P_arr[k - 1] * ((seq_t)(5 * (k - 1) + 2));
        }

        /* Compute next Stirling row: st_curr[0] = 0 */
        st_curr[0] = 0;
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t term = st_prev[k - 1];
            seq_t mul = st_prev[k];
            mul *= (cur - 1);
            st_curr[k] = term + mul;
        }

        /* Sum over k to get a(cur) */
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t prod = P_arr[k] * st_curr[k];
            if (((cur & 1) == (k & 1))) {
                sum_pos += prod;
            } else {
                sum_neg += prod;
            }
        }

        cache[cur] = sum_pos - sum_neg;

        /* Prepare for next iteration */
        seq_t *tmp = st_prev;
        st_prev = st_curr;
        st_curr = tmp;
    }

    last_computed = n;
    return cache[n];
}
