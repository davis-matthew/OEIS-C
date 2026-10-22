/*
 * A341575  E.g.f.: log(1 - log(1 - x))^2 / 2.
 * https://oeis.org/A341575
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=2..n} |Stirling1(n,k)| · Stirling1(k,2) (unsigned
 *            Stirling of first kind for s(n,k) and s(k,2). The computation
 *            uses O(n²) incremental construction of Stirling numbers and
 *            stores only unsigned values, storing the sign of s(k,2) as a
 *            factor of (-1)^{k-2}
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stddef.h>
#include <stdlib.h>

/* Utility headers for dynamic allocation of seq_t arrays */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cache state */
static idx_t   max_computed = 0;      /* largest n for which a(n) is known   */
static size_t  cur_cap      = 0;      /* current allocated capacity           */

static seq_t  *prev_row = NULL;       /* Stirling numbers of row n-1         */
static seq_t  *curr_row = NULL;       /* buffer for current row n            */
static seq_t  *c_k2    = NULL;        /* unsigned |Stirling1(k,2)|           */
static seq_t  *fact    = NULL;        /* factorials                         */
static seq_t  *a_cache = NULL;        /* cached a(n) values                 */

/* Grow arrays to at least indices [0..needed] and compute all a(k) for
   max_computed+1 <= k <= needed.  */
static void grow_to(idx_t needed)
{
    if (needed <= max_computed) return;                /* nothing to do */

    idx_t new_cap = needed + 1;                        /* include 0 */

    if (!prev_row) {                                  /* first time */
        prev_row = seq_alloc((size_t)new_cap);
        curr_row = seq_alloc((size_t)new_cap);
        c_k2    = seq_alloc((size_t)new_cap);
        fact    = seq_alloc((size_t)new_cap);
        a_cache = seq_alloc((size_t)new_cap);
        cur_cap = (size_t)new_cap;

        /* --- initialise base case ---- */
        prev_row[0] = 0;
        prev_row[1] = 1;                 /* s(1,1) = 1 */
        fact[0] = 1;
        fact[1] = 1;
        c_k2[0] = 0;
        c_k2[1] = 0;
        a_cache[0] = 0;
        a_cache[1] = 0;
        max_computed = 1;                /* known up to n=1 */
    } else {                                      /* enlarge existing arrays */
        size_t old_cap = cur_cap;
        prev_row = seq_realloc(prev_row, old_cap, (size_t)new_cap);
        curr_row = seq_realloc(curr_row, old_cap, (size_t)new_cap);
        c_k2    = seq_realloc(c_k2,    old_cap, (size_t)new_cap);
        fact    = seq_realloc(fact,    old_cap, (size_t)new_cap);
        a_cache = seq_realloc(a_cache, old_cap, (size_t)new_cap);
        cur_cap = (size_t)new_cap;
        for (idx_t i = old_cap; i < new_cap; ++i) {
            prev_row[i] = 0;
            curr_row[i] = 0;
            c_k2[i]     = 0;
            fact[i]     = 0;
            a_cache[i]  = 0;
        }
    }

    /* --- build rows and compute a(n) for new n --- */
    for (idx_t m = max_computed + 1; m <= needed; ++m) {
        /*  factorial(m) = factorial(m-1) * m */
        fact[m] = fact[m - 1] * (seq_t)m;

        /*  |Stirling1(m,2)| :  s(m,2) = (m-1) * s(m-1,2) + (m-2)!   */
        if (m == 2) {
            c_k2[m] = 1;                /* (2-1)! = 1; H(1) = 1 */
        } else {
            c_k2[m] = ((seq_t)(m - 1)) * c_k2[m - 1] + fact[m - 2];
        }

        /*  Stirling numbers of the first kind (unsigned) for row m */
        curr_row[0] = 0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t part1 = 0;
            if (k <= m - 1) part1 = ((seq_t)(m - 1)) * prev_row[k];
            seq_t part2 = prev_row[k - 1];
            curr_row[k] = part1 + part2;
        }

        /*  accumulate a(m) = Σ_{k=2..m} (-1)^{k-2} * |s(m,k)| * |s(k,2)|   */
        seq_t sum_even = 0;          /* k even  -> factor +1   */
        seq_t sum_odd  = 0;          /* k odd   -> factor -1   */
        for (idx_t k = 2; k <= m; ++k) {
            seq_t prod = curr_row[k] * c_k2[k];   /* unsigned product   */
            if ((k & 1) == 0) {
                sum_even += prod;
            } else {
                sum_odd += prod;
            }
        }
        a_cache[m] = sum_even - sum_odd;   /* odd terms subtracted */

        /* ---- prepare for next iteration ---- */
        seq_t *tmp = prev_row;
        prev_row   = curr_row;
        curr_row   = tmp;
    }

    max_computed = needed;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                   /* offset is 2 */
    grow_to(n);
    return a_cache[n];
}
