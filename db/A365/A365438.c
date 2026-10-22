/*
 * A365438  E.g.f. satisfies A(x) = 1 - log(1 - x*A(x)^2)/A(x).
 * https://oeis.org/A365438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A365438: a(n) = Sum_{k=0..n} (2*n-k)!/(2*n-2*k+1)! * |Stirling1(n,k)| */

#include <stdlib.h>

/* dynamic array helper: ensure arr has space for at least needed+1 elements */
static void
ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap <= needed) {
        size_t new_cap = (needed + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        /* zero‑initialise the new part */
        for (size_t i = *cap; i < new_cap; ++i) tmp[i] = 0;
        *arr = tmp;
        *cap = new_cap;
    }
}

/* compute a(n) for any n >= 0.  The function is memoised and works in O(n) time
 * overall, using only integer operations. */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static seq_t *st         = NULL;   /* Stirling numbers: |s(cur, k)| */
    static size_t st_cap     = 0;
    static idx_t  max_computed = (idx_t)-1;

    /* initialise on first call */
    if (!cache) {
        size_t init = (n > 0) ? (size_t)n + 1 : 1;
        cache = (seq_t *)calloc(init, sizeof(seq_t));
        st    = (seq_t *)calloc(init, sizeof(seq_t));
        if (!cache || !st) abort();
        cache_cap = init;
        st_cap    = init;
        cache[0] = (seq_t)1;    /* a(0) = 1 */
        st[0]    = (seq_t)1;    /* |s(0,0)| = 1 */
        max_computed = 0;
    }

    /* if we already know this term, just return it */
    if (n <= max_computed) return cache[(size_t)n];

    /* grow arrays if necessary */
    ensure_capacity(&cache, &cache_cap, (size_t)n);
    ensure_capacity(&st,    &st_cap,    (size_t)n);

    /* compute terms incrementally from max_computed + 1 up to n */
    for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
        /* update Stirling numbers for current cur */
        for (idx_t k = cur; k >= 1; --k) {
            st[k] = st[k-1] + (seq_t)(cur - 1) * st[k];
        }
        st[0] = (seq_t)0;   /* |s(cur,0)| = 0 for cur > 0 */

        /* compute the sum for this cur */
        seq_t sum = 0;
        seq_t f   = 1;              /* f_k, start with k=1 */
        seq_t two_n = (seq_t)cur * 2;

        for (idx_t k = 1; k <= cur; ++k) {
            if (k > 1) {
                /* recurrence: f_k = f_{k-1} * (2n-2k+3)*(2n-2k+2)/(2n-k+1) */
                seq_t mul1 = two_n - (seq_t)(2 * k - 3);  /* 2n-2k+3 */
                seq_t mul2 = two_n - (seq_t)(2 * k - 2);  /* 2n-2k+2 */
                seq_t div  = two_n - (seq_t)(k - 1);      /* 2n-k+1 */
                f = (f * mul1 * mul2) / div;
            }
            sum += f * st[k];
        }

        cache[(size_t)cur] = sum;
        max_computed = cur;
    }

    return cache[(size_t)n];
}
