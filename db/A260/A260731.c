/*
 * A260731  a(n) = Number of steps to reach 0 starting from x=n and using the
 *          iterated process: x -> x - A002828(x), where A002828(x) = the
 *          least number of squares that add up to x.
 * https://oeis.org/A260731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; a(n)=1+a(n-A002828(n)) for n>0, where A002828(n) is the
 *            least number of squares that sum to n
 * Range:     u8 n <= 710, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#endif

/* Global storage for minimal number of squares (g_arr) and step counts (step_arr). */
static seq_t *g_arr   = NULL; /* g_arr[i] = minimal number of squares that sum to i */
static seq_t *step_arr = NULL; /* step_arr[i] = a(i) */
static idx_t  cap      = 0;    /* number of entries currently stored (>= 0) */

static void ensure_data(idx_t n)
{
    if (n < cap)
        return;

    idx_t new_cap = n + 1;

#ifdef __cplusplus
    if (g_arr == NULL)
        g_arr = (seq_t*)seq_alloc(new_cap);
    else
        g_arr = (seq_t*)seq_realloc(g_arr, cap, new_cap);

    if (step_arr == NULL)
        step_arr = (seq_t*)seq_alloc(new_cap);
    else
        step_arr = (seq_t*)seq_realloc(step_arr, cap, new_cap);
#else
    g_arr = (seq_t*)realloc(g_arr, new_cap * sizeof(seq_t));
    step_arr = (seq_t*)realloc(step_arr, new_cap * sizeof(seq_t));
#endif

    for (idx_t i = cap; i < new_cap; ++i)
    {
        if (i == 0)
        {
            g_arr[0] = (seq_t)0;
            step_arr[0] = (seq_t)0;
            continue;
        }

        /* Compute the least number of squares for i.  */
        seq_t best = (seq_t)i;  /* upper bound, will become <=4 */
        for (idx_t j = 1; j * j <= i; ++j)
        {
            seq_t cand = g_arr[i - j * j] + (seq_t)1;
            if (cand < best)
                best = cand;
        }
        g_arr[i] = best;

        /* Compute the step count for i: x -> x - best */
        idx_t prev = i - (idx_t)best;
        step_arr[i] = step_arr[prev] + (seq_t)1;
    }
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;
    ensure_data(n);
    return step_arr[n];
}
