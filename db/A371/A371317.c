/*
 * A371317  E.g.f. satisfies A(x) = (exp(x) - 1)/(1 - A(x))^3.
 * https://oeis.org/A371317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (4k-2)!/(3k-1)! * Stirling2(n,k). The
 *            product (4k-2)!/(3k-1)! equals product_{i=3k}^{4k-2} i
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* --- sequence implementation -------------------------------------------- */

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *coeff  = NULL;   /* c[k] = (4k-2)!/(3k-1)!  */
    static seq_t *S      = NULL;   /* Stirling numbers of 2nd kind for current n */
    static seq_t *cache  = NULL;   /* a(k) values computed so far */
    static size_t coeff_cap = 0, S_cap = 0, cache_cap = 0;
    static idx_t  max_comp = (idx_t)-1;  /* highest n that has been computed */

    /* initialise on first call */
    if (max_comp == (idx_t)-1) {
        coeff_cap = 2;               /* store c[0] and c[1] */
        coeff  = (seq_t*)realloc(coeff, coeff_cap * sizeof(seq_t));
        coeff[0] = 0;                /* unused, keep at index 0 */
        coeff[1] = 1;                /* (4*1-2)!/(3*1-1)! = 1 */

        S_cap = 2;                   /* indices 0..1 */
        S = (seq_t*)realloc(S, S_cap * sizeof(seq_t));
        S[0] = 1;                   /* S(0,0)=1 */
        S[1] = 0;                   /* S(1,1)=0 initially, will be set for n=1 */

        cache_cap = 1;               /* cache[0] only */
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        cache[0] = 0;                /* a(0)=0 */

        max_comp = 0;
    }

    /* grow data structures and compute new values up to requested n */
    while (max_comp < n) {
        idx_t new_n = max_comp + 1;

        /* ---- compute coefficient c[new_n] --------------------------------- */
        if (new_n >= coeff_cap) {
            size_t new_cap = new_n + 1;
            coeff = (seq_t*)realloc(coeff, new_cap * sizeof(seq_t));
            coeff_cap = new_cap;
        }
        if (new_n == 1) {
            coeff[1] = 1;
        } else {
            seq_t val = 1;
            for (idx_t i = 3 * new_n; i <= 4 * new_n - 2; ++i)
                val *= (seq_t)i;
            coeff[new_n] = val;
        }

        /* ---- grow Stirling array ------------------------------------------ */
        if (new_n >= S_cap) {
            size_t new_cap = new_n + 1;
            S = (seq_t*)realloc(S, new_cap * sizeof(seq_t));
            S_cap = new_cap;
        }

        /* ---- grow cache array --------------------------------------------- */
        if (new_n >= cache_cap) {
            size_t new_cap = new_n + 1;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* ---- update Stirling numbers for the new row n = new_n ------------- */
        for (idx_t k = new_n; k >= 1; --k)
            S[k] = k * S[k] + S[k - 1];
        S[0] = 0;  /* for n > 0 the zeroth column is 0 */

        /* ---- compute a(new_n) --------------------------------------------- */
        seq_t sum = 0;
        for (idx_t k = 1; k <= new_n; ++k)
            sum += coeff[k] * S[k];
        cache[new_n] = sum;

        max_comp = new_n;
    }

    return cache[n];
}
