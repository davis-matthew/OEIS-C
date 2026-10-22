/*
 * A366564  Expansion of e.g.f. -log(1 - x^2 * (exp(x) - 1)).
 * https://oeis.org/A366564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..floor(n/3)} (k-1)! *
 *            Stirling2(n-2*k,k)/(n-2*k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by the recurrence derived from the e.g.f.:
   a(0)=0, a(1)=0, a(2)=0, and for n ≥ 2,
   a(n+1) = n*(n+1) + Σ_{t=1}^{n-2} C(n, t-1)*(n-t+1)*(n-t)*a(t). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* number of allocated elements */
    static idx_t max_computed = 0;  /* highest index computed (>=2 after init) */

    /* ---------- helper: grow cache if necessary ----------
       Cache holds indices 0..max_cache-1.  We need at least n+1 entries. */
    if (n + 1 > cap) {
        size_t newcap = n + 1;
        if (newcap < 16) newcap = 16;          /* small default */
        seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!newptr) { /* allocation failed: abort function return 0 */ }
        cache = newptr;
        cap = newcap;
    }

    /* ---------- initialise base cases if not already ----------
       a(0)=a(1)=a(2)=0.  These are trivial and independent of cache capacity. */
    if (max_computed < 2) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        max_computed = 2;
    }

    /* ---------- compute values up to n if needed ----------
       The recurrence uses all a(t) for t <= n-2, so we iterate from
       the smallest required index up to n-1.  Each iteration calculates
       a[i+1] where i is the current loop index. */
    if (n > max_computed) {
        for (idx_t i = max_computed; i <= n - 1; ++i) {
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;        /* C(i,0) */

            /* inner sum over t = 1 .. i-2 */
            for (idx_t t = 1; t <= i - 2; ++t) {
                /* binom currently equals C(i, t-1) */
                seq_t at = cache[t];
                if (at) {
                    seq_t term = at * binom;
                    /* multiply by (i - t + 1) * (i - t)  (both fit into 32‑bit) */
                    term = term * (seq_t)(i - t + 1) * (seq_t)(i - t);
                    sum += term;
                }
                /* advance to next binomial coefficient C(i, t) */
                seq_t mult = binom * (seq_t)(i - t + 1);
                binom = mult / (seq_t)(t);   /* exact division */
            }

            /* b_i = i * (i + 1) for the RHS term */
            seq_t bi = (seq_t)i * (seq_t)(i + 1);
            cache[i + 1] = bi + sum;
        }
        max_computed = n;
    }

    return cache[n];
}
