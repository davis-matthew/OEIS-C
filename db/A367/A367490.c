/*
 * A367490  Expansion of e.g.f. -x * log(4 - 3*exp(x)).
 * https://oeis.org/A367490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Sum_{k=1..n-1} 3^k * (k-1)! * Stirling2(n-1,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0, a(1) = 0 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special cases for offset */
    if (n == 0 || n == 1)
        return (seq_t)0;

    /* Static state for memoisation */
    static seq_t *S_prev   = NULL;
    static seq_t *S_curr   = NULL;
    static size_t   S_cap  = 0;

    static seq_t *a_cache  = NULL;
    static size_t   A_cap  = 0;
    static idx_t    A_max  = 1;   /* largest n for which a_cache[n] is computed */

    /* Ensure capacity for a_cache and Stirling rows */
    size_t need_cap = (size_t)n + 1;    /* we need indices up to n */
    if (need_cap > A_cap) {
        seq_t *tmp = (seq_t *)realloc(a_cache, need_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        a_cache = tmp;
        A_cap   = need_cap;
        /* initialise new entries to 0 (not strictly necessary but safe) */
        for (size_t i = A_cap; i < need_cap; ++i) a_cache[i] = (seq_t)0;
        /* initialise base terms */
        a_cache[0] = (seq_t)0;
        a_cache[1] = (seq_t)0;
    }

    if (need_cap > S_cap) {
        seq_t *p1 = (seq_t *)realloc(S_prev, need_cap * sizeof(seq_t));
        seq_t *p2 = (seq_t *)realloc(S_curr, need_cap * sizeof(seq_t));
        if (!p1 || !p2) return (seq_t)0;
        S_prev = p1;
        S_curr = p2;
        S_cap  = need_cap;
        if (A_max == 1) {          /* first allocation */
            S_prev[0] = (seq_t)1;  /* S(0,0) = 1 */
        }
    }

    /* If already computed, return the cached value */
    if (n <= A_max)
        return a_cache[n];

    /* Compute rows from A_max upto n-1 */
    for (idx_t m = A_max; m <= n - 1; ++m) {
        /* Ensure S_prev[0..m] exist (array is large enough by allocation above) */
        S_curr[0] = (seq_t)0;     /* S(m,0) = 0 for m >= 1 */

        /* u128 used for intermediate calculations */
        unsigned __int128 sum = 0;
        unsigned __int128 fact = 1;          /* (k-1)!  for k=1 is 1 */
        unsigned __int128 power3 = 3;        /* 3^k  for k=1 */

        for (idx_t k = 1; k <= m; ++k) {
            /* Stirling recurrence: S(m,k) = k * S(m-1,k) + S(m-1,k-1) */
            unsigned __int128 s_prev_k   = (k <= m - 1) ? (unsigned __int128)S_prev[k] : 0;
            unsigned __int128 s_prev_k1  = (unsigned __int128)S_prev[k - 1];
            unsigned __int128 s = (unsigned __int128)k * s_prev_k + s_prev_k1;

            S_curr[k] = (seq_t)s;

            sum += s * power3 * fact;

            fact *= k;              /* next factorial (k)! becomes (k+1-1)!   */
            power3 *= 3;            /* next power of 3 */
        }

        unsigned __int128 a_val = (unsigned __int128)(m + 1) * sum;
        a_cache[m + 1] = (seq_t)a_val;

        /* Prepare for next iteration */
        seq_t *tmp = S_prev;
        S_prev = S_curr;
        S_curr = tmp;
    }

    A_max = n;
    return a_cache[n];
}
