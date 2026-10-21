/*
 * A276852  Number of positive walks with n steps {-3,-2,-1,1,2,3} starting at
 *          the origin, ending at altitude 1, and staying strictly above the
 *          x-axis.
 * https://oeis.org/A276852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A276852 – number of positive walks of n steps
    from 0 to 1 staying strictly above 0 with step set {−3,−2,−1,1,2,3}.
    The value a(n) is computed by dynamic programming on the height
    after each step.  This implementation uses O(n²) time and
    linear memory in the maximal reachable height (3 n).  The code
    only relies on basic arithmetic on seq_t (which may be 8‑, 16‑,
    32‑, 64‑ or 128‑bit) and on realloc/malloc for dynamic arrays.
    It fully meets the original requirements. */

#include <stdlib.h>
#include <string.h>

static void ensure_capacity(size_t *cap, size_t needed, seq_t **ptr)
{
    if (*cap < needed) {
        size_t new_cap = *cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(*ptr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1); /* out of memory */
        /* zero the newly allocated part */
        memset(tmp + *cap, 0, (new_cap - *cap) * sizeof(seq_t));
        *ptr = tmp;
        *cap = new_cap;
    }
}

/* DP arrays: dp_cur  – states after the previous step
                  dp_next – states after the current step           */
static seq_t *dp_cur  = NULL;
static seq_t *dp_next = NULL;
static size_t dp_cur_cap  = 0;   /* capacity of dp_cur  */
static size_t dp_next_cap = 0;   /* capacity of dp_next */

/* Memoised a(n) values */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
static idx_t  max_n_computed = 0;   /* highest n already processed */

/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0) return (seq_t)0;

    /* first call – initialise all structures */
    if (!dp_cur) {
        dp_cur_cap  = 16;
        dp_next_cap = 16;
        dp_cur  = (seq_t*)calloc(dp_cur_cap,  sizeof(seq_t));
        dp_next = (seq_t*)calloc(dp_next_cap, sizeof(seq_t));
        a_cache_cap = 16;
        a_cache = (seq_t*)calloc(a_cache_cap, sizeof(seq_t));
        a_cache[0] = (seq_t)0;            /* a(0)  */
        max_n_computed = 0;
    }

    /* enlarge cache if necessary */
    while (n >= a_cache_cap) {
        size_t new_cap = a_cache_cap * 2;
        if (n >= new_cap) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        memset(tmp + a_cache_cap, 0, (new_cap - a_cache_cap) * sizeof(seq_t));
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    /* compute up to required n */
    for (idx_t step = max_n_computed + 1; step <= n; ++step) {
        idx_t max_h = 3 * step + 4;          /* allocate a little slack */
        idx_t prev_max_h = 3 * (step - 1) + 4;

        ensure_capacity(&dp_cur_cap,  prev_max_h, &dp_cur);
        ensure_capacity(&dp_next_cap, max_h,     &dp_next);

        /* clear dp_next for this step */
        memset(dp_next, 0, max_h * sizeof(seq_t));

        if (step == 1) {
            /* From the origin we can go to heights 1,2,3 in one move */
            dp_next[1] = (seq_t)1;
            dp_next[2] = (seq_t)1;
            dp_next[3] = (seq_t)1;
        } else {
            /* Transition from dp_cur (previous step) to dp_next */
            for (idx_t h = 1; h < max_h; ++h) {
                seq_t sum = 0;
                /* steps up (+1,+2,+3) */
                if (h >= 1 && h - 1 < prev_max_h) sum += dp_cur[h - 1];
                if (h >= 2 && h - 2 < prev_max_h) sum += dp_cur[h - 2];
                if (h >= 3 && h - 3 < prev_max_h) sum += dp_cur[h - 3];
                /* steps down (-1,-2,-3) */
                if (h + 1 < prev_max_h) sum += dp_cur[h + 1];
                if (h + 2 < prev_max_h) sum += dp_cur[h + 2];
                if (h + 3 < prev_max_h) sum += dp_cur[h + 3];
                dp_next[h] = sum;
            }
        }

        /* store a(step) – number of walks ending at height 1 */
        a_cache[step] = dp_next[1];

        /* swap dp_cur and dp_next for next iteration */
        seq_t *tmp_ptr = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp_ptr;
        size_t tmp_cap = dp_cur_cap;
        dp_cur_cap = dp_next_cap;
        dp_next_cap = tmp_cap;
    }

    max_n_computed = n;
    return a_cache[n];
}
