/*
 * A275445  Sum of the asymmetry degrees of all compositions of n with parts
 *          in {1,2,3}.
 * https://oeis.org/A275445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 69, u128 n <= 141,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is a(n)=∑_{x,y∈{1,2,3}, x+y≤n}
 *       a(n‑x‑y) + [x≠y]·f(n‑x‑y),
 * where f(k) is the number of compositions of k with parts in {1,2,3}
 * (f(0)=1, f(k)=f(k‑1)+f(k‑2)+f(k‑3) for k≥1).
 * Initial values: a(0)=0, a(1)=0, a(2)=0.
 * This implements the recurrence in O(n) per call, with memoisation. */

#include <stddef.h>
#include "../utils/seq_realloc.h"   /* seq_realloc(sequence*, old_cap, new_cap) */

/* Memoised arrays for f (composition counts) and a (desired sequence). */
static seq_t *f_arr      = NULL;
static seq_t *dp_arr     = NULL;
static size_t arr_cap    = 0;          /* current allocated size */
static idx_t  computed   = 0;          /* first index not yet computed */

seq_t a(idx_t n)
{
    /* Ensure the memoisation arrays are large enough.  */
    if (n + 1 > arr_cap) {
        size_t new_cap = (size_t)n + 1;
        f_arr = seq_realloc(f_arr, arr_cap, new_cap);
        dp_arr = seq_realloc(dp_arr, arr_cap, new_cap);
        arr_cap = new_cap;
    }

    /* Compute all missing values up to n. */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        /* Base case for f: f[0]=1; other f[i] defined recursively. */
        if (i == 0) {
            f_arr[i] = (seq_t)1;
            dp_arr[i] = (seq_t)0;
            continue;
        }

        /* Compute f[i] = f[i-1] + f[i-2] + f[i-3] (missing terms treated as 0). */
        f_arr[i] = f_arr[i - 1];
        if (i >= 2) f_arr[i] += f_arr[i - 2];
        if (i >= 3) f_arr[i] += f_arr[i - 3];

        /* Recurrence for dp[i]. */
        seq_t val = (seq_t)0;
        for (int x = 1; x <= 3; ++x) {
            for (int y = 1; y <= 3; ++y) {
                int sumxy = x + y;
                if ((size_t)sumxy > i) continue;
                size_t m = i - (size_t)sumxy;      /* remaining inner weight */
                val += dp_arr[m];                  /* add inner asymmetry sum */
                if (x != y)                        /* add mismatch contribution */
                    val += f_arr[m];
            }
        }
        dp_arr[i] = val;
    }

    computed = (idx_t)n + 1;
    return dp_arr[(size_t)n];
}
