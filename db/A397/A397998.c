/*
 * A397998  E.g.f. A(x) satisfies A((exp(4*x) - 1)/4) = 5*A(x) - 4*x.
 * https://oeis.org/A397998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = Sum_{k=1..n} 4^(n-k) * Stirling2(n+1,k) *
 *            a(k) (Stirling numbers of the second kind, S(n,k))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build environment. */
static seq_t *a_arr = NULL;
static seq_t *s_prev = NULL;
static seq_t *s_cur  = NULL;
static idx_t computed_max = 0;   /* largest n for which a_arr[n] is computed */
static size_t capacity = 0;      /* array capacity in elements */

/* Ensure that arrays are large enough to store values up to n (1‑based). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 2 <= capacity)
        return;
    size_t newcap = (size_t)n + 2;
    a_arr  = (seq_t *)realloc(a_arr, newcap * sizeof(seq_t));
    s_prev = (seq_t *)realloc(s_prev, newcap * sizeof(seq_t));
    s_cur  = (seq_t *)realloc(s_cur,  newcap * sizeof(seq_t));
    capacity = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset is 1; behaviour for n<1 is irrelevant */

    ensure_capacity(n);

    /* initialise the first entry if this is the first call */
    if (computed_max == 0) {
        a_arr[1] = (seq_t)1;
        /* S(1,1) = 1 */
        s_prev[1] = (seq_t)1;
        computed_max = 1;
    }

    /* compute progressively up to the requested n */
    for (idx_t m = computed_max; m < n; ++m) {
        /* m is the current row index (we have a(m) and S(m,*) ready) */
        /* compute the next Stirling row: S(m+1, k) for k=1..m+1  */
        s_cur[1] = (seq_t)1;                  /* S(m+1,1) = 1 */
        for (idx_t k = 2; k <= m + 1; ++k) {
            seq_t term1 = (k <= m) ? s_prev[k] * (seq_t)k : (seq_t)0;
            seq_t term2 = s_prev[k - 1];
            s_cur[k] = term1 + term2;
        }

        /* compute a(m+1) using the recurrence */
        seq_t pow4 = (seq_t)1;                 /* 4^(m-k), start with k=m */
        seq_t sum  = (seq_t)0;
        for (idx_t k = m; k >= 1; --k) {
            seq_t prod = s_cur[k] * a_arr[k];
            prod *= pow4;
            sum += prod;
            pow4 *= (seq_t)4;
            if (k == 1) break;   /* avoid unsigned wrap around */
        }
        a_arr[m + 1] = sum;

        /* prepare for next iteration */
        seq_t *tmp = s_prev;
        s_prev = s_cur;
        s_cur  = tmp;
    }

    computed_max = n;
    return a_arr[n];
}
