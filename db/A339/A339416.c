/*
 * A339416  Number of compositions (ordered partitions) of n into an even
 *          number of triangular numbers.
 * https://oeis.org/A339416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula:
 *  a(n) = Number of compositions (ordered partitions) of n into an even number
 *         of triangular numbers.
 *  The implementation uses dynamic programming with parity tracking.
 */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* static arrays storing counts for exact parity */
static seq_t *dp_even = NULL;   /* compositions with even #parts */
static seq_t *dp_odd  = NULL;   /* compositions with odd  #parts */
/* current capacity of the arrays (size) */
static idx_t  dp_cap  = 0;
static idx_t  computed_max = 0; /* highest n for which dp values are computed */

/* ensure dp arrays have space for indices up to n */
static void ensure_dp(idx_t n)
{
    if (n + 1 <= dp_cap)
        return;

    idx_t new_cap = n + 1;
    seq_t *new_even = (seq_t *)realloc(dp_even, new_cap * sizeof(seq_t));
    seq_t *new_odd  = (seq_t *)realloc(dp_odd,  new_cap * sizeof(seq_t));
    if (!new_even || !new_odd)
        abort();

    dp_even = new_even;
    dp_odd  = new_odd;

    /* initialise the newly allocated cells to zero */
    for (idx_t i = dp_cap; i < new_cap; ++i) {
        dp_even[i] = 0;
        dp_odd [i] = 0;
    }
    dp_cap = new_cap;

    /* ensure base case is present */
    if (dp_even[0] == 0) {
        dp_even[0] = 1;   /* empty composition has even length (0 parts) */
        dp_odd [0] = 0;
    }
}

/* compute dp values for indices (computed_max+1 .. n) */
static void compute_to(idx_t n)
{
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t ev = 0;
        seq_t od = 0;
        for (idx_t k = 1;; ++k) {
            /* use 128‑bit arithmetic to avoid overflow for triangular numbers */
            unsigned __int128 t128 = (unsigned __int128)k * (k + 1) / 2;
            if (t128 > i)
                break;
            idx_t t = (idx_t)t128;
            ev += dp_odd[i - t];    /* appending one part flips parity */
            od += dp_even[i - t];
        }
        dp_even[i] = ev;
        dp_odd [i] = od;
    }
    computed_max = n;
}

/* public sequence function: a(idx_t n) */
seq_t a(idx_t n)
{
    ensure_dp(n);
    if (computed_max < n)
        compute_to(n);
    return dp_even[n];
}
