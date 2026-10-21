/*
 * A247082  Expansion of e.g.f. (8 - 7*cosh(x)) / (13 - 12*cosh(x)).
 * https://oeis.org/A247082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247082
   Formula: a(n) = Sum_{k=1..2n} 2^{k-1} * k! * Stirling2(2n, k),  a(0)=1
   (Stirling numbers of the second kind, recursive definition)
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* --- static cache for a(n) values ------------------------------------ */
    static seq_t *a_cache   = NULL;   /* array of computed a() values        */
    static size_t cache_size = 0;     /* allocated size of a_cache           */
    static idx_t  cache_used = 0;     /* largest index whose value is known  */

    /* --- static tables for Stirling numbers and factorials --------------- */
    static seq_t *S        = NULL;   /* current row of Stirling numbers     */
    static seq_t *fact     = NULL;   /* factorials up to current m          */
    static size_t S_size   = 0;      /* allocated size of S and fact arrays */
    static idx_t  max_m    = 0;      /* largest m already processed         */

    /* --- handle the trivial case ---------------------------------------- */
    if (n == 0) return (seq_t)1;

    /* --- initial allocation for a_cache --------------------------------- */
    if (a_cache == NULL) {
        cache_size = 1;
        a_cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!a_cache) return (seq_t)0;       /* out of memory, return 0 */
        a_cache[0] = (seq_t)1;               /* a(0) = 1 */
        cache_used = 0;
    }

    /* --- return if already known ---------------------------------------- */
    if (n <= cache_used)
        return a_cache[n];

    /* --- ensure cache can hold the requested index ----------------------- */
    if ((idx_t)n >= cache_size) {
        size_t new_cap = cache_size ? cache_size * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        a_cache = tmp;
        cache_size = new_cap;
    }

    /* --- target m = 2*n --------------------------------------------------- */
    idx_t target_m = n * 2;

    /* --- ensure arrays for Stirling numbers and factorials are large ---- */
    size_t needed = (size_t)target_m + 1;
    if (needed > S_size) {
        size_t new_size = S_size ? S_size * 2 : 64;
        while (new_size < needed) new_size <<= 1;
        seq_t *tS  = (seq_t *)realloc(S,     new_size * sizeof(seq_t));
        seq_t *tF  = (seq_t *)realloc(fact,  new_size * sizeof(seq_t));
        if (!tS || !tF) return (seq_t)0;
        S = tS;  fact = tF;
        /* zero the new part of S (factorials are set when needed) */
        for (idx_t i = S_size; i < new_size; ++i) S[i] = (seq_t)0;
        S_size = new_size;
    }

    /* --- initial Stirling row for m=0 if not already done ----------------- */
    if (max_m == 0) {
        S[0]    = (seq_t)1;   /* S(0,0)=1 */
        fact[0] = (seq_t)1;   /* 0!   =1 */
    }

    /* --- build rows from max_m+1 up to target_m -------------------------- */
    for (idx_t m = max_m + 1; m <= target_m; ++m) {
        /* factorial */
        fact[m] = fact[m - 1] * (seq_t)m;

        /* Stirling numbers: S(m,k) = k*S(m-1,k) + S(m-1,k-1)  (k runs descending) */
        for (idx_t k = m; k > 0; --k) {
            S[k] = S[k] * (seq_t)k + S[k - 1];
        }
        S[0] = (seq_t)0;  /* S(m,0)=0 for m>0 */

        /* when m is even we have calculated a(n) for n = m/2 ----------------- */
        if ((m & 1ULL) == 0) {
            idx_t n0 = m >> 1;          /* n0 = m/2 */

            /* ensure cache can hold this value (already done above) */

            /* compute the sum  Σ_{k=1..m} 2^{k-1} * k! * S(m,k) */
            seq_t sum = (seq_t)0;
            seq_t power2 = (seq_t)1;    /* 2^{0} */
            for (idx_t k = 1; k <= m; ++k) {
                seq_t term = S[k] * fact[k];
                term = term * power2;
                sum += term;
                power2 <<= 1;
            }
            a_cache[n0] = sum;
            if (n0 > cache_used) cache_used = n0;
        }
    }

    max_m = target_m;
    return a_cache[n];
}
