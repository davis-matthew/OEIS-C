/*
 * A390434  Triangle read by rows, Lah numbers with level 3.
 * https://oeis.org/A390434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where row = ⎣(√(1+8n)-1)/2⎦, col = n -
 *            row*(row+1)/2. Recurrence: T(n,k) = T(n-1,k-1) + (n+k-1)^3 *
 *            T(n-1,k), with T(n,n)=1, T(n,0)=0 for n>0, T(0,0)=1. The
 *            sequence a(n) is the triangle read by rows
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 28, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include necessary headers */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper function: integer square root of a 64‑bit number.
   Using int_sqrt from utils if available, otherwise a simple binary search. */
static inline idx_t int_sqrt_uint64(idx_t n) {
#if defined(__GNUC__) || defined(__clang__)
    /* GCC/Clang provide __builtin_sqrtll for 64‑bit, but we don't need it.
       We'll use a quick binary search for portability. */
#endif
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* Global static state for memoized incremental computation */
static seq_t *cache = NULL;          /* Flattened a(n) values */
static size_t cache_len = 0;         /* Number of computed a(n) */
static size_t cache_cap = 0;         /* Allocated capacity for cache */

static seq_t *prev_row = NULL;       /* Previous row values */
static size_t prev_size = 0;         /* Size of prev_row (row_index+1) */
static idx_t max_row_done = 0;       /* Highest row that has been fully computed */

/* Ensure cache has room for at least 'need' elements */
static void ensure_cache_capacity(size_t need) {
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure: abort (should not happen in test harness) */
            exit(1);
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Entry point: compute a(n) */
seq_t a(idx_t n) {
    /* Fast path: already computed */
    if (cache_len > n) {
        return cache[n];
    }

    /* If first call, initialize row 0 */
    if (cache_len == 0) {
        /* Allocate space for row 0 */
        ensure_cache_capacity(1);
        prev_row = (seq_t *)malloc(sizeof(seq_t));
        if (!prev_row) exit(1);
        prev_row[0] = (seq_t)1;      /* T(0,0) */
        cache[0] = prev_row[0];
        cache_len = 1;
        prev_size = 1;
        max_row_done = 0;
    }

    /* Compute rows until cache_len > n */
    while (cache_len <= n) {
        idx_t new_row = max_row_done + 1;    /* Index of new row to compute */
        size_t curr_size = (size_t)new_row + 1;   /* Size of row array */

        /* Allocate current row */
        seq_t *curr_row = (seq_t *)malloc(curr_size * sizeof(seq_t));
        if (!curr_row) exit(1);

        /* First element is 0 for n>0 */
        if (new_row > 0)
            curr_row[0] = (seq_t)0;

        /* Compute middle elements */
        for (size_t k = 1; k < new_row; ++k) {
            seq_t term   = prev_row[k];        /* T(n-1,k) */
            seq_t add    = prev_row[k - 1];    /* T(n-1,k-1) */
            seq_t factor = (seq_t)(new_row + k - 1);
            factor = factor * factor * factor; /* (n+k-1)^3 */
            curr_row[k] = add + factor * term;
        }

        /* Last element is 1 */
        curr_row[new_row] = (seq_t)1;

        /* Append row elements to cache */
        ensure_cache_capacity(cache_len + curr_size);
        for (size_t k = 0; k < curr_size; ++k) {
            cache[cache_len++] = curr_row[k];
        }

        /* Prepare for next iteration */
        free(prev_row);
        prev_row = curr_row;
        prev_size = curr_size;
        ++max_row_done;
    }

    /* Return requested value */
    return cache[n];
}
