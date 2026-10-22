/*
 * A364385  a(n) is the number of quadratic equations u*x^2 + v*x + w = 0 with
 *          different solution sets L != {}, where n >= abs(u) + abs(v) +
 *          abs(w) and the coefficients u, v, w as well as the solutions x_1,
 *          x_2 are integers.
 * https://oeis.org/A364385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A364385: a(n) = number of unordered integer pairs (x,y) with
 * 1 + |x+y| + |x*y| <= n, i.e. the number of quadratic equations
 * u·x²+v·x+w=0 with integer coefficients and integer solutions,
 * counted up to an overall sign of (u,v,w).  The function computes the
 * cumulative counts up to the requested n and memoises the result for
 * later calls. */

#include <stdint.h>
#include <stdlib.h>

#ifndef __cplusplus
/* In C the seq_t and idx_t types are supplied by the build. */
#endif

static seq_t *prefix = NULL;          /* cumulative counts a(k) for k <= prefix_cap-1 */
static idx_t  prefix_cap = 0;          /* allocated size (max n + 1) */

/* Absolute value for 64‑bit signed integers */
static inline int64_t abs64(int64_t x) { return x < 0 ? -x : x; }

/* Helper: allocate an array of seq_t, handling C++ and C */
static seq_t *alloc_seq_array(idx_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)calloc((size_t)n, sizeof(seq_t));
#endif
}

/* Helper: free an array allocated by alloc_seq_array */
static void free_seq_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    /* allocate or grow the memoisation buffer if necessary */
    if (n + 1 > prefix_cap)
    {
        idx_t new_cap = n + 1;

        /* Allocate frequency array for minimal costs m (stored in 1‑based).
         * The maximum possible cost is at most n (we already know that).
         */
        seq_t *freq = alloc_seq_array(new_cap);
        if (!freq) return 0;  /* allocation failure */

        /* Count unordered pairs (x,y) for this n, using |x|,|y| <= n-1. */
        int64_t max_xy = (int64_t)n - 1;          /* maximal |x| or |y| when product = 0 */

        for (int64_t x = -max_xy; x <= max_xy; ++x)
        {
            /* Find maximum |y| allowed by |x*y| <= n-1. */
            int64_t max_abs_y = max_xy;
            if (x != 0)
            {
                int64_t absx = abs64(x);
                int64_t temp = (max_xy) / absx;
                if (temp < max_abs_y) max_abs_y = temp;
            }

            /* y must be >= x to keep unordered pairs. */
            int64_t y_start = x;
            if (y_start < -max_abs_y) y_start = -max_abs_y;   /* ensure lower bound */
            if (y_start > max_abs_y) continue;              /* no valid y */

            for (int64_t y = y_start; y <= max_abs_y; ++y)
            {
                int64_t prod = x * y;
                /* If the product exceeds the bound, stop the loop early
                 * because |x*y| grows monotonically with |y| for fixed x. */
                if (abs64(prod) > (int64_t)n - 1) break;

                int64_t sum = x + y;
                int64_t cost = 1 + abs64(sum) + abs64(prod);
                if (cost <= (int64_t)n)
                    freq[(size_t)cost] += 1;  /* freq[0] stays zero */
            }
        }

        /* Build cumulative prefix array: a(k) = sum_{m=1..k} freq[m] */
        seq_t *new_prefix = alloc_seq_array(new_cap);
        if (!new_prefix) { free_seq_array(freq); return 0; }

        seq_t acc = 0;
        for (idx_t i = 0; i < new_cap; ++i)
        {
            acc += freq[(size_t)i];
            new_prefix[i] = acc;
        }

        /* Replace old memoisation buffer */
        if (prefix) free_seq_array(prefix);
        prefix = new_prefix;
        prefix_cap = new_cap;

        free_seq_array(freq);
    }

    return prefix[n];
}
