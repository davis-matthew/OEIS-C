/*
 * A357084  E.g.f. satisfies log(A(x)) = (exp(x*A(x)) - 1)^2 * A(x).
 * https://oeis.org/A357084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A357084
 * Formula: a(n) = Sum_{k=0..⌊n/2⌋} (2*k)! * (n+k+1)^(k-1) * Stirling2(n,2*k)/k!
 *
 * Recurrence:  S(n,m) = m*S(n-1,m) + S(n-1,m-1)   (Stirling numbers of the second kind)
 * with S(0,0)=1 and S(n,0)=0 for n>0.
 *
 * Implementation: we build successive rows of Stirling numbers and accumulate
 * the sum.  The term (2k)!/k! is replaced by the product
 *   Π_{i=k+1}^{2k} i
 * which avoids intermediate divisions and keeps 64‑bit intermediates small
 * for all n whose a(n) fits in a 64‑bit unsigned integer.
 *
 * The function is memoised: any previously computed a(n) is stored in a
 * dynamically sized cache so that calls may appear in arbitrary order.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_pow.h"

/* Sequence entry point */
seq_t a(idx_t n)
{
    /* ---------- memoisation & cache allocation ----------------------------------- */
    static idx_t  max_computed = 0;     /* largest n for which a(n) is already stored   */
    static seq_t *cache       = NULL;   /* cache[0..max_computed] with a(n)             */
    static size_t cache_cap   = 0;       /* allocated capacity of the cache             */

    /* Allocate or extend cache to hold values up to the requested n */
    if (cache == NULL) {
        cache_cap = (size_t)1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        max_computed = 0;
    } else if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* If the requested n is already cached, return it */
    if (n < max_computed + 1)
        return cache[n];

    /* ---------- Stirling number tables ------------------------------------------ */
    static seq_t *S_prev = NULL;   /* previous row S(n-1, m)  */
    static seq_t *S_cur  = NULL;   /* current row S(n,   m)   */
    static size_t S_cap   = 0;      /* allocated capacity for both rows   */

    size_t needed_S_cap = (size_t)n + 1;
    if (S_cap < needed_S_cap) {
        S_cur  = (seq_t*)realloc(S_cur,  needed_S_cap * sizeof(seq_t));
        S_prev = (seq_t*)realloc(S_prev, needed_S_cap * sizeof(seq_t));
        S_cap  = needed_S_cap;
    }

    /* Initialise S(0,0)=1, S(0,m)=0 for m>0 */
    if (max_computed == 0) {
        S_prev[0] = (seq_t)1;
        for (size_t i = 1; i < S_cap; ++i) S_prev[i] = (seq_t)0;
    }

    /* ---------- Compute rows up to n and corresponding a(n) values --------------- */
    for (idx_t cur = max_computed + 1; cur <= n; ++cur)
    {
        /* Build Stirling numbers for row 'cur' from row 'cur-1' stored in S_prev */
        S_cur[0] = (seq_t)0;
        for (idx_t m = 1; m < cur; ++m)
            S_cur[m] = (seq_t)m * S_prev[m] + S_prev[m - 1];
        S_cur[cur] = S_prev[cur - 1];   /* S(cur,cur) = 1 */

        /* Compute a(cur) via the given summation */
        seq_t sum = (cur == 0) ? (seq_t)1 : (seq_t)0;

        idx_t maxk = cur / 2;
        for (idx_t k = 1; k <= maxk; ++k)
        {
            seq_t power = seq_pow((seq_t)(cur + k + 1), k - 1);   /* (n+k+1)^(k-1) */

            /* S = S(cur, 2k) */
            seq_t S = S_cur[2 * k];

            /* term = S * power * product_{i=k+1}^{2k} i */
            seq_t term = S * power;

            /* Multiply by the ratio (2k)! / k! = Π i=k+1..2k i */
            for (idx_t i = k + 1; i <= 2 * k; ++i)
                term *= (seq_t)i;

            sum += term;
        }

        cache[cur] = sum;

        /* Shift rows for next iteration */
        seq_t *tmp = S_prev;
        S_prev = S_cur;
        S_cur  = tmp;
    }

    max_computed = n;
    return cache[n];
}
