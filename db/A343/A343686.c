/*
 * A343686  a(0) = 1; a(n) = 3 * n * a(n-1) + Sum_{k=0..n-1} binomial(n,k) *
 *          (n-k-1)! * a(k).
 * https://oeis.org/A343686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A343686: a(0)=1; a(n)=3*n*a(n-1)+∑_{k=0}^{n-1} C(n,k)*(n-k-1)!*a(k) */
/* a(0)=1, a(1)=4, a(2)=33, ... (see OEIS entry for further terms) */

#include <stdlib.h>

/* a(idx_t n) returns the n-th term of the sequence */
seq_t a(idx_t n)
{
    /* --- Static cache for computed terms -------------------------------- */
    static seq_t *cache      = NULL;   /* cache[0..computed-1] */
    static size_t   cap      = 0;      /* allocated size of cache   */
    static size_t   computed = 0;      /* number of terms cached    */

    /* --- Static factorial table: fact[0]..fact[n-1]  > 0 for n>0 ---------*/
    static seq_t *fact      = NULL;   /* factorial table           */
    static size_t   fact_cnt = 0;     /* number of factorials stored */

    /* Helper to grow cache as needed */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (cap == 0) ? 1 : cap;
        while (newcap <= (size_t)n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* Grow factorial table to size n (need fact[0..n-1]) */
    if (n > 0 && fact_cnt < n) {
        size_t newcnt = (fact_cnt == 0) ? 1 : fact_cnt;
        while (newcnt < n) newcnt <<= 1;
        fact = (seq_t *)realloc(fact, newcnt * sizeof(seq_t));

        /* Initialise factorials from previously stored point */
        if (fact_cnt == 0) {
            fact[0] = (seq_t)1;
            fact_cnt = 1;
        }
        for (size_t i = fact_cnt; i < n; ++i) {
            fact[i] = fact[i-1] * (seq_t)i;
        }
        fact_cnt = n;
    }

    /* Initialise a(0) if not done */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t m = computed; m <= n; ++m) {
        /* m == 0 never happens here because computed==1 after init */
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;        /* C(m,0) */

        for (idx_t k = 0; k < m; ++k) {
            seq_t term = binom * fact[m - k - 1] * cache[k];
            sum += term;

            /* Update binom to C(m, k+1) if needed */
            if (k + 1 < m) {
                seq_t num = (seq_t)(m - k);
                seq_t den = (seq_t)(k + 1);
                binom = binom * num;
                binom = binom / den;    /* exact division */
            }
        }

        seq_t part = (seq_t)3 * (seq_t)m * cache[m - 1];
        cache[m] = part + sum;
    }

    computed = (size_t)n + 1;
    return cache[n];
}
