/*
 * A382830  a(n) = Sum_{k=0..n} binomial(n+k-1,k) * |Stirling1(n,k)| * k!.
 * https://oeis.org/A382830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A382830
   Formula: a(n) = Σ_{k=0..n} binomial(n+k-1,k) * |Stirling1(n,k)| * k!
   Equivalent: a(n) = Σ_{k=0..n} |S1(n,k)| * (n)_k   (rising factorial)
   The implementation memoises Stirling numbers of the first kind (absolute
   values) row by row and accumulates the sum for every n requested.  It
   works for any build where seq_t is an unsigned integer type (including
   arbitrary‑precision), and for any order of calls. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Growth helper for dynamic arrays of seq_t. */
static void grow_seq(seq_t **pptr, size_t *cap, size_t new_cap)
{
    if (new_cap <= *cap)
        return;
    if (*pptr == NULL)
        *pptr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    else
        *pptr = (seq_t*)realloc(*pptr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Global memoised state. */
static bool     initialized = false;
static idx_t   cur_n        = 0;            /* highest n processed */
static seq_t  *s_prev       = NULL;         /* |S1(cur_n,k)| for k=0..cur_n   */
static size_t  s_prev_cap   = 0;
static seq_t  *s_cur        = NULL;         /* scratch for next row           */
static size_t  s_cur_cap    = 0;
static seq_t  *cache        = NULL;         /* a(n) values for n=0..cache_cap-1 */
static size_t  cache_cap    = 0;

/* Ensure that a(n) is available in cache and all Stirling rows up to n
   have been computed.  The routine can be called in any order. */
static void ensure_cache(idx_t n)
{
    if (!initialized) {
        /* Initialisation: row n=0, a(0)=1 */
        grow_seq(&s_prev, &s_prev_cap, 1);
        s_prev[0] = (seq_t)1;               /* |S1(0,0)| = 1                 */
        grow_seq(&s_cur,  &s_cur_cap,  1);
        s_cur[0]  = (seq_t)0;

        grow_seq(&cache, &cache_cap, 1);
        cache[0]  = (seq_t)1;               /* a(0) = 1                       */

        cur_n      = 0;
        initialized = true;
    }

    for (idx_t m = cur_n; m < n; ++m) { /* m runs 0..n-1, newN = m+1 */
        idx_t newN = m + 1;

        /* Ensure scratch arrays hold newN+1 entries */
        grow_seq(&s_prev, &s_prev_cap, newN + 1);
        grow_seq(&s_cur,  &s_cur_cap,  newN + 1);

        /* Compute |S1(newN,k)| for k=0..newN */
        s_cur[0] = (seq_t)0;
        for (idx_t k = 1; k <= newN; ++k) {
            seq_t val = s_prev[k - 1];           /* S1(m,k-1) term */
            if (k <= m)                          /* add m * S1(m,k) if exists */
                val = val + (seq_t)m * s_prev[k];
            s_cur[k] = val;
        }

        /* Compute a(newN) = Σ |S1(newN,k)| * rising factorial (newN)_k */
        seq_t sum    = (seq_t)0;
        seq_t rising = (seq_t)1;                 /* (newN)_0 = 1 */
        for (idx_t k = 0; k <= newN; ++k) {
            seq_t term = s_cur[k];
            if (k > 0)
                term = term * rising;          /* multiply by (newN)_k */
            sum = sum + term;
            if (k < newN)
                rising = rising * ( (seq_t)(newN + k) );
        }

        /* Store result */
        grow_seq(&cache, &cache_cap, newN + 1);
        cache[newN] = sum;

        /* Prepare for next iteration: swap rows */
        seq_t *tmp  = s_prev;
        s_prev      = s_cur;
        s_cur       = tmp;
    }
    cur_n = n;
}

/* Public sequence function required by the kernel. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
