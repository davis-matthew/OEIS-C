/*
 * A377359  E.g.f. satisfies A(x) = ( 1 - log(1 - x*A(x))/A(x) )^3.
 * https://oeis.org/A377359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * Σ_{k=0..⌊(3n+3)/4⌋} (3n-3k+2)!/(3n-4k+3)! · |s(n,k)|
 *            where s(n,k) are unsigned Stirling numbers of the first kind
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Declare the required function with external linkage */
seq_t a(idx_t n);

/* ----------   Internal static state   ---------- */
static seq_t *a_cache      = NULL;   /* cache for a(n) values   */
static size_t a_cache_cap = 0;       /* capacity of a_cache     */

static seq_t *s_prev = NULL;   /* previous row of Stirling numbers */
static seq_t *s_cur  = NULL;   /* current   row of Stirling numbers */
static size_t prev_cap = 0;
static size_t cur_cap  = 0;
static idx_t  max_n_computed = 0;   /* highest n already computed */

/* Ensure that *ptr points to an array with at least 'needed' elements.
   Reallocates if necessary.  Compiles in C and C++ (C++ casts void*). */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (*cap < needed) {
        seq_t *newp = (seq_t *)realloc(*ptr, needed * sizeof(seq_t));
        if (!newp) abort();           /* abort on allocation failure */
        *ptr = newp;
        *cap = needed;
    }
}

/* Compute a(n) using the explicit formula */
seq_t a(idx_t n)
{
    /* ----- First‑time initialization ----- */
    if (a_cache == NULL) {
        a_cache_cap = 1;
        a_cache = (seq_t *)malloc(sizeof(seq_t) * a_cache_cap);
        if (!a_cache) abort();
        a_cache[0] = (seq_t)1;          /* a(0) = 1 */

        /* Stirling row 0: only s(0,0)=1 */
        prev_cap = 1;
        s_prev = (seq_t *)malloc(sizeof(seq_t) * prev_cap);
        if (!s_prev) abort();
        s_prev[0] = (seq_t)1;

        max_n_computed = 0;
    }

    /* ----- Return cached value if already computed ----- */
    if (n <= max_n_computed)
        return a_cache[n];

    /* ----- Compute subsequent rows up to n ----- */
    for (idx_t cur = max_n_computed + 1; cur <= n; ++cur) {
        /* Make sure current row buffer has enough space */
        if (!s_cur)
            cur_cap = 0;
        ensure_capacity((size_t)cur + 1, &cur_cap, &s_cur);
        s_cur[0] = 0;  /* s(cur,0) = 0 for cur > 0 */

        /* ----- Stirling recurrence: s(cur,k) = s(prev,k-1) + (cur-1)*s(prev,k) ----- */
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t term1 = s_prev[k - 1];
            seq_t term2 = 0;
            if (k <= max_n_computed)                /* s_prev holds row cur-1 */
                term2 = s_prev[k] * (seq_t)(cur - 1);
            s_cur[k] = term1 + term2;
        }

        /* ----- Accumulate a(cur) from Stirling numbers ----- */
        seq_t sum = 0;
        if (cur == 0) {
            sum = (seq_t)1;                /* a(0) = 1 */
        } else {
            idx_t Kmax = (3 * cur + 3) / 4;
            for (idx_t k = 1; k <= Kmax; ++k) {
                seq_t t = s_cur[k];
                for (idx_t i = 0; i < k - 1; ++i) {
                    seq_t factor = (seq_t)(3 * cur - 4 * k + 4 + i);
                    t = t * factor;
                }
                t = t * (seq_t)3;
                sum += t;
            }
        }

        /* ----- Store the computed value ----- */
        ensure_capacity((size_t)cur + 1, &a_cache_cap, &a_cache);
        a_cache[cur] = sum;

        /* ----- Prepare for next iteration: swap row buffers ----- */
        seq_t *tmp = s_prev;
        s_prev = s_cur;
        s_cur = tmp;
        size_t tmp_cap = prev_cap;
        prev_cap = cur_cap;
        cur_cap = tmp_cap;

        max_n_computed = cur;
    }

    return a_cache[n];
}
