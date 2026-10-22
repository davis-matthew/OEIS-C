/*
 * A317172  a(n) = n! * [x^n] 1/(1 - n*log(1 + x)).
 * https://oeis.org/A317172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Stirling1(n,k) * n^k * k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static arrays for Stirling numbers (current row) and result cache   */
    static seq_t *s = NULL;
    static idx_t s_cap = 0;
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t last = 0;   /* largest n already computed */

    /* --- ensure base case (n == 0) is initialized -------------------------------- */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;           /* a(0) = 1 */
        s_cap = 1;
        s = (seq_t *)malloc(s_cap * sizeof(seq_t));
        s[0] = (seq_t)1;               /* Stirling(0,0) = 1 */
        last = 0;
    }

    if (n == 0) return cache[0];

    /* --- grow cache array if necessary ------------------------------------------ */
    if ((size_t)n >= cache_cap) {
        size_t newcap = cache_cap? cache_cap : 1;
        while (newcap <= (size_t)n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }

    /* --- grow Stirling array if necessary ---------------------------------------- */
    if ((size_t)(last + 1) > s_cap) {
        size_t newcap = s_cap? s_cap : 1;
        while (newcap <= (size_t)(last + 1)) newcap <<= 1;
        s = (seq_t *)realloc(s, newcap * sizeof(seq_t));
        s_cap = newcap;
    }

    /* --- compute values up to requested n --------------------------------------- */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        /* ensure space for s[i] */
        if (s_cap <= i) {
            size_t newcap = s_cap? s_cap : 1;
            while (newcap <= (size_t)i) newcap <<= 1;
            s = (seq_t *)realloc(s, newcap * sizeof(seq_t));
            s_cap = newcap;
        }
        s[i] = (seq_t)0;                     /* new element before update */

        /* in‑place Stirling recurrence:  s[n][k] = s[n-1][k-1] - (n-1)*s[n-1][k] */
        for (idx_t k = i; k >= 1; --k)
        {
            seq_t old_prev = s[k-1];
            seq_t old_cur  = s[k];
            s[k] = old_prev - ((seq_t)(i-1)) * old_cur;
        }
        s[0] = (seq_t)0;                     /* Stirling(n,0) = 0 for n>0 */

        /* compute a(i) via the summation */
        seq_t total  = (seq_t)0;
        seq_t term   = (seq_t)1;   /* n^0 * 0! = 1 */

        for (idx_t k = 0; k <= i; ++k)
        {
            if (k > 0) {
                seq_t mult = ((seq_t)i) * ((seq_t)k);   /* multiply once by n*k */
                term = term * mult;
            }
            seq_t prod = s[k] * term;
            total = total + prod;
        }
        cache[i] = total;
    }
    last = n;
    return cache[n];
}
