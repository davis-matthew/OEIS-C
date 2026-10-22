/*
 * A367047  G.f. satisfies A(x) = 1 - x^3 + x*A(x)^4.
 * https://oeis.org/A367047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A367047
 *  Recurrence derived from the generating function
 *      A(x) = 1 - x^3 + x*A(x)^4
 *
 *  The series coefficients satisfy
 *      a(0) = 1
 *      a(n) = Σ_{p+q = n-1} s[p]·s[q]   (n≠3)
 *      a(3) = Σ_{p+q = 2} s[p]·s[q] – 1
 *
 *  where s[k] = Σ_{i+j = k} a[i]·a[j]  (ordered pairs).
 *  The values are generated incrementally; the arrays a[] and s[]
 *  are stored in static caches so successive calls for arbitrary
 *  indices are efficient.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static tables of computed terms and pair sums */
    static seq_t *a_cache  = NULL;   /* a[0..]        */
    static seq_t *s_cache  = NULL;   /* s[0..]        */
    static size_t  a_cap   = 0;       /* capacity of a_cache   */
    static size_t  s_cap   = 0;       /* capacity of s_cache   */
    static idx_t   computed_upto = 0; /* largest n already computed */

    /* Initialise one‑time */
    if (a_cap == 0) {
        a_cap = 1;
        a_cache = (seq_t*)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = 1;              /* a(0) = 1 */

        s_cap = 1;
        s_cache = (seq_t*)malloc(s_cap * sizeof(seq_t));
        s_cache[0] = 1;              /* s[0] = a0*a0 = 1 */

        computed_upto = 0;
    }

    /* Ensure a_cache can hold up to n */
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                 /* allocation failed */
        if (new_cap > a_cap)
            memset((void*)(tmp + a_cap), 0,
                   (new_cap - a_cap) * sizeof(seq_t));
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* Ensure s_cache can hold indices up to 2*n (used in updates) */
    if ((size_t)2 * n + 1 > s_cap) {
        size_t new_cap = (size_t)2 * n + 1;
        seq_t *tmp = (seq_t*)realloc(s_cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        if (new_cap > s_cap)
            memset((void*)(tmp + s_cap), 0,
                   (new_cap - s_cap) * sizeof(seq_t));
        s_cache = tmp;
        s_cap   = new_cap;
    }

    /* Compute all a(i) up to requested n */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        /* Σ_{p+q=i-1} s[p]·s[q] */
        seq_t sum = 0;
        for (idx_t p = 0; p <= i - 1; ++p)
            sum += s_cache[p] * s_cache[i - 1 - p];

        if (i == 3)        /* subtract the -x^3 term */
            sum -= 1;

        a_cache[i] = sum;

        /* Update s_cache with new pairs involving a[i] */
        for (idx_t j = 0; j < i; ++j) {
            seq_t add = a_cache[i] * a_cache[j];
            add += add;            /* factor 2 for ordered (i,j) and (j,i) */
            s_cache[i + j] += add;
        }
        /* pair (i,i) contributes once */
        s_cache[2 * i] += a_cache[i] * a_cache[i];
    }

    computed_upto = n;
    return a_cache[n];
}
