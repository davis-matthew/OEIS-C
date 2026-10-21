/*
 * A296982  Expansion of e.g.f. arctanh(log(1 + x)).
 * https://oeis.org/A296982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296982: Expansion of e.g.f. arctanh(log(1 + x))
   a(n) = Σ_{k odd} (k-1)! * S(n,k)
   where S(n,k) are Stirling numbers of the second kind.
   The function computes the term a(n) for any n ≥ 0. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache and working storage */
    static seq_t *s_prev = NULL;   /* previous row of s(n,k) */
    static seq_t *s_cur  = NULL;   /* current row   */
    static seq_t *fact   = NULL;   /* factorials (k-1)! */
    static seq_t *a_cache= NULL;   /* a(n) values    */
    static idx_t max_n = 0;        /* largest n computed so far */
    static size_t s_cap = 0;      /* capacity of s arrays   */
    static size_t fact_cap = 0;   /* capacity of fact array */
    static size_t a_cap = 0;      /* capacity of a_cache    */

    /* initial allocation for n==0 */
    if (max_n == 0 && a_cache == NULL)
    {
        /* allocate minimal size (1 element) */
        s_prev  = (seq_t *)malloc(sizeof(seq_t));
        s_cur   = (seq_t *)malloc(sizeof(seq_t));
        fact    = (seq_t *)malloc(sizeof(seq_t));
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!s_prev || !s_cur || !fact || !a_cache)
            return (seq_t)0;  /* out of memory */

        /* initialise base values */
        s_prev[0] = (seq_t)1;          /* s(0,0)=1 */
        fact[0]   = (seq_t)1;          /* 0!  = 1  */
        a_cache[0]= (seq_t)0;          /* a(0)=0   */

        s_cap    = 1;
        fact_cap = 1;
        a_cap    = 1;
        max_n    = 0;
    }

    /* if we already have the requested value return it immediately */
    if (n < max_n)
        return a_cache[n];

    /* enlarge factorial array if necessary */
    if (n + 1 > fact_cap)
    {
        fact = (seq_t *)realloc(fact, (n + 1) * sizeof(seq_t));
        if (!fact)
            return (seq_t)0; /* allocation failed */
        for (idx_t i = fact_cap; i <= n; ++i)
        {
            if (i == 0)
                fact[i] = (seq_t)1;
            else
                fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_cap = n + 1;
    }

    /* enlarge s arrays if necessary */
    if (n + 1 > s_cap)
    {
        s_prev = (seq_t *)realloc(s_prev, (n + 1) * sizeof(seq_t));
        s_cur  = (seq_t *)realloc(s_cur,  (n + 1) * sizeof(seq_t));
        if (!s_prev || !s_cur)
            return (seq_t)0; /* allocation failed */
        s_cap = n + 1;
    }

    /* enlarge cache for a(n) */
    if (n + 1 > a_cap)
    {
        a_cache = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
        if (!a_cache)
            return (seq_t)0; /* allocation failed */
        a_cap = n + 1;
    }

    /* compute rows for n = max_n+1 ... n */
    for (idx_t cur = max_n + 1; cur <= n; ++cur)
    {
        /* first element is always zero for cur>0 */
        s_cur[0] = (seq_t)0;

        /* compute current row s(cur,k) for k=1..cur */
        for (idx_t k = 1; k <= cur; ++k)
        {
            seq_t term1 = s_prev[k - 1];   /* s(cur-1, k-1) */
            seq_t term2 = (seq_t)0;
            if (k <= cur - 1)                /* s(cur-1, k) exists? */
                term2 = s_prev[k] * (seq_t)(cur - 1);
            s_cur[k] = term1 - term2;
        }

        /* compute a(cur) = Σ_{k odd} (k-1)! * s(cur,k) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= cur; k += 2)
            sum += fact[k - 1] * s_cur[k];
        a_cache[cur] = sum;

        /* swap s_prev and s_cur for next iteration */
        seq_t *tmp   = s_prev;
        s_prev      = s_cur;
        s_cur       = tmp;
    }

    max_n = n;
    return a_cache[n];
}
