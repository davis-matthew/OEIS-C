/*
 * A358494  a(n) = Sum_{k=0..floor(n/5)} (n-4*k)!/k!.
 * https://oeis.org/A358494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/5⌋} (n-4k)!/k! (OEIS A358494)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- Memoisation for a(n) ---------- */
    static seq_t *cache  = NULL;  /* cache[0..cache_len-1] contains a(0)...a(cache_len-1) */
    static idx_t  cache_len = 0;  /* number of cached terms */

    /* ---------- Memoisation for factorials ---------- */
    static seq_t *fact   = NULL;  /* factorials: fact[i] = i!  */
    static idx_t  fact_len = 0;   /* number of stored factorials: fact[0..fact_len-1]  */

    /* ----- Ensure factorials up to n are prepared ----- */
    if (n + 1 > fact_len) {
        idx_t new_fact_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(fact, new_fact_len * sizeof(seq_t));
        fact = tmp;
        if (fact_len == 0) {          /* initialise first entry if this is the first call */
            fact[0] = (seq_t)1;
            fact_len = 1;
        }
        for (idx_t i = fact_len; i <= n; ++i) {
            /* fact[i] = fact[i-1] * i   (i is small enough to cast to seq_t) */
            fact[i] = fact[i-1] * (seq_t)i;
        }
        fact_len = new_fact_len;
    }

    /* ----- Compute a(i) for all i from cache_len to n ---------- */
    if (n >= cache_len) {
        idx_t new_cache_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cache_len * sizeof(seq_t));
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            /* Compute a(i) directly using the defining sum.  */
            idx_t maxk = i / 5;          /* floor(i/5) */
            seq_t sum = 0;
            seq_t term = fact[i];        /* (i-0)! / 0!   */
            sum = term;

            for (idx_t k = 1; k <= maxk; ++k) {
                /* Update term from previous k-1 to k.      */
                /* term = term / k / (i-4k+4) / (i-4k+3) / (i-4k+2) / (i-4k+1)   */
                term = term / (seq_t)k;

                seq_t a1 = (seq_t)(i - 4*k + 4);
                seq_t a2 = (seq_t)(i - 4*k + 3);
                seq_t a3 = (seq_t)(i - 4*k + 2);
                seq_t a4 = (seq_t)(i - 4*k + 1);

                term = term / a1;
                term = term / a2;
                term = term / a3;
                term = term / a4;

                sum += term;
            }
            cache[i] = sum;
        }
        cache_len = new_cache_len;
    }

    return cache[n];
}
