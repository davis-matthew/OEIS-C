/*
 * A341588  E.g.f.: -log(1 + log(1 - x))^3 / 6.
 * https://oeis.org/A341588
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=3..n} |Stirling1(n, k) * Stirling1(k, 3)|
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a dynamic program for unsigned Stirling
 * numbers of the first kind and accumulates the required sums.
 * It works for any seq_t that supports the usual arithmetic operators. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Static cache state */
static idx_t current_n = 0;              /* Highest n for which a(n) is cached */
static seq_t *prev_row = NULL;           /* Stirling row for n-1   */
static seq_t *cur_row  = NULL;           /* Stirling row for n     */
static seq_t *sto3     = NULL;           /* S1(k,3) values for k  */
static seq_t *a_cache  = NULL;           /* Cached a(k) values    */
static size_t cap_prev = 0;
static size_t cap_cur  = 0;
static size_t cap_sto3 = 0;
static size_t cap_acache = 0;

/* Ensure an array has capacity at least "need" */
static void ensure_cap(size_t *cap, seq_t **arr, size_t need)
{
    if (*cap < need) {
        size_t new_cap = need;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* Out of memory */
        /* Zero the newly allocated part */
        if (new_cap > *cap)
            memset(tmp + *cap, 0, (new_cap - *cap) * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* Values below the offset are undefined */

    /* Grow caches if needed */
    if (n > current_n) {
        size_t needed = (size_t)n + 1;
        ensure_cap(&cap_prev, &prev_row, needed);
        ensure_cap(&cap_cur,  &cur_row,  needed);
        ensure_cap(&cap_sto3, &sto3,     needed);
        ensure_cap(&cap_acache,&a_cache, needed);

        /* First time initialization */
        if (current_n == 0) {
            memset(prev_row, 0, needed * sizeof(seq_t));
            prev_row[0] = 1;   /* S1(0,0) = 1 */
            /* sto3 and a_cache already zeroed */
        }
    }

    /* Extend sequence up to n */
    for (size_t i = (size_t)current_n + 1; i <= (size_t)n; ++i) {
        /* Compute row i using prev_row (which is row i-1) */
        cur_row[0] = 0;   /* S1(i,0) = 0 */
        for (size_t k = 1; k <= i; ++k) {
            seq_t term1 = prev_row[k - 1];
            seq_t term2 = 0;
            if (k <= i - 1) {
                term2 = prev_row[k] * (seq_t)(i - 1);
            }
            cur_row[k] = term1 + term2;
        }

        /* Store S1(i,3) for later use; zero for i < 3 */
        if (i >= 3) sto3[i] = cur_row[3];
        else        sto3[i] = 0;

        /* Compute a(i) = Sum_{k=3..i} S1(i,k) * S1(k,3) */
        seq_t sum = 0;
        for (size_t k = 3; k <= i; ++k) {
            sum += cur_row[k] * sto3[k];
        }
        a_cache[i] = sum;

        /* Prepare for next iteration: swap rows */
        seq_t *tmp = prev_row;
        prev_row = cur_row;
        cur_row = tmp;
    }

    current_n = (size_t)n;
    return a_cache[n];
}
