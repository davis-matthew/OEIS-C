/*
 * A348793  G.f. A(x) satisfies A(x) = (1 + x * A(x)^3) / (1 - 2 * x).
 * https://oeis.org/A348793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * a(n) = 2 * a(n-1) + * Sum_{i=0..n-1}
 *            Sum_{j=0..n-i-1} a(i)*a(j)*a(n-i-j-1) * This is equivalent to
 *            the given recurrence and generates OEIS * sequence A348793. The
 *            implementation uses memoisation so that * every call works
 *            independently and in any order
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* forward declaration of seq_t helpers (provided by build) */
extern void seq_free_array(seq_t*);

/* memoised cache structures */
static seq_t *a_cache = NULL;      /* holds a[0..max_n]          */
static seq_t *s_cache = NULL;      /* holds s[0..max_n]          */
static size_t  a_cap   = 0;        /* allocated capacity for a   */
static size_t  s_cap   = 0;        /* allocated capacity for s   */
static idx_t   max_n   = 0;        /* highest index computed     */

/* ensure array capacity */
static void ensure_cap(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        if (new_cap < needed) new_cap = needed;
        seq_t *ptr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = ptr;
        *cap = new_cap;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* initialise cache if first call */
    if (a_cache == NULL) {
        ensure_cap(&a_cache, &a_cap, 1);
        ensure_cap(&s_cache, &s_cap, 1);
        /* a(0) = 1 */
        a_cache[0] = 1;
        s_cache[0] = 1;      /* s[0] = a(0)*a(0) */
        max_n = 0;
    }

    /* extend cache up to requested n */
    while (max_n < n) {
        idx_t m = max_n + 1;   /* next index to compute */

        /* compute c_{m-1} = Σ_{i=0}^{m-1} a[i] * s[m-1-i] */
        seq_t c = 0;
        for (idx_t i = 0; i < m; ++i) {
            c += a_cache[i] * s_cache[m - 1 - i];
        }

        /* a[m] = 2*a[m-1] + c */
        seq_t next = a_cache[max_n] * 2 + c;

        /* ensure capacities */
        ensure_cap(&a_cache, &a_cap, m + 1);
        ensure_cap(&s_cache, &s_cap, m + 1);

        a_cache[m] = next;

        /* compute s[m] = Σ_{i=0}^{m} a[i] * a[m-i] */
        seq_t sm = 0;
        for (idx_t i = 0; i <= m; ++i) {
            sm += a_cache[i] * a_cache[m - i];
        }
        s_cache[m] = sm;

        max_n = m;
    }

    return a_cache[n];
}
