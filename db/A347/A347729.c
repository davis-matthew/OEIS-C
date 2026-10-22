/*
 * A347729  Number of compositions (ordered partitions) of n into at most 6
 *          cubes.
 * https://oeis.org/A347729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into at most 6 cubes
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#define MAX_K 6   /* maximum number of parts */

/* static state for memoization */
static seq_t **dp = NULL;       /* dp[k][n] = #compositions of n into exactly k cubes */
static seq_t *a_arr = NULL;     /* a_arr[n] = a(n) */
static idx_t capacity = 0;      /* current allocated capacity (not exceeding max N seen) */

/* cube values up to a given limit */
static idx_t cube_vals[100];
static size_t cube_cnt = 0;

/* Ensure dp and a_arr are allocated and filled up to index 'needed' (inclusive). */
static void ensure_capacity(idx_t needed)
{
    if (needed < capacity)
        return;                /* already computed up to 'needed' */

    /* Determine new capacity: at least needed + 1, grow to reduce reallocs */
    idx_t new_cap;
    if (capacity == 0)
        new_cap = 1;
    else
        new_cap = capacity * 2;
    if (new_cap <= needed)
        new_cap = needed + 1;

    /* Allocate dp rows if necessary. */
    if (!dp) {
        dp = (seq_t **)malloc((MAX_K + 1) * sizeof(seq_t *));
        for (int k = 0; k <= MAX_K; ++k)
            dp[k] = NULL;
    }
    for (int k = 0; k <= MAX_K; ++k) {
        if (!dp[k])
            dp[k] = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            dp[k] = (seq_t *)realloc(dp[k], new_cap * sizeof(seq_t));
    }

    /* Allocate a_arr if necessary. */
    if (!a_arr)
        a_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));

    /* Compute cube values up to new_cap-1. */
    cube_cnt = 0;
    for (idx_t c = 1;; ++c) {
        idx_t val = c * c * c;
        if (val >= new_cap)
            break;
        cube_vals[cube_cnt++] = val;
    }

    /* Fill new entries for indices from 'capacity' to 'new_cap-1'. */
    for (idx_t n = capacity; n < new_cap; ++n) {
        dp[0][n] = (n == 0 ? (seq_t)1 : (seq_t)0); /* base case */

        for (int k = 1; k <= MAX_K; ++k) {
            seq_t sum = 0;
            for (size_t i = 0; i < cube_cnt; ++i) {
                idx_t c = cube_vals[i];
                if (c > n)
                    break;
                sum += dp[k - 1][n - c];
            }
            dp[k][n] = sum;
        }

        /* a_arr[n] = sum over k=0..MAX_K of dp[k][n] */
        seq_t total = 0;
        for (int k = 0; k <= MAX_K; ++k)
            total += dp[k][n];
        a_arr[n] = total;
    }

    capacity = new_cap;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    return a_arr[n];
}
