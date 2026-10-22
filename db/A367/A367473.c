/*
 * A367473  Expansion of e.g.f. 1 / (4 - 3 * exp(x))^3.
 * https://oeis.org/A367473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A367473: Expansion of e.g.f. 1 / (4 - 3 * exp(x))^3
   Recurrence (used):
     a(0) = 1
     a(n) = 3 * Σ_{k=1..n} ( (2*k + n)/n ) * binomial(n,k) * a(n-k)   (n ≥ 1)
   The summand is always an integer; the division by n is exact.
   The function memoises previously computed terms in a static dynamic array,
   so subsequent calls may reuse earlier values.
   The implementation uses only integer arithmetic, malloc/realloc, and
   the types seq_t and idx_t supplied by the build system. */

#include <stdlib.h>   /* for malloc, realloc, free, memset */

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;          /* capacity of cache array */

    /* allocate initial cache if needed */
    if (cap == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;        /* allocation failure -> zero */
        cache[0] = (seq_t)1;         /* a(0) = 1 */
        cap = 1;
    }

    /* grow cache to hold index n, if necessary */
    if (n >= cap) {
        idx_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= n) new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure -> zero */
        /* zero the newly added entries */
        for (idx_t i = cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* return memoised value if already computed (non‑zero) */
    if (cache[n] != 0) return cache[n];
    if (n == 0) return cache[0];

    seq_t sum = 0;
    /* binomial(n, k) is updated multiplicatively */
    seq_t binom = (seq_t)n;          /* binomial(n,1) */

    for (idx_t k = 1; k <= n; ++k) {
        /* term = binom * a[n-k] * (2*k + n) / n */
        seq_t term = binom * cache[n - k];
        term = term * ((seq_t)(2 * k + n));
        term = term / (seq_t)n;
        sum += term;

        /* update binom to binomial(n, k+1) for next iteration */
        if (k < n) {
            binom = binom * (seq_t)(n - k);
            binom = binom / (seq_t)(k + 1);
        }
    }

    /* a(n) = 3 * sum */
    cache[n] = sum * (seq_t)3;
    return cache[n];
}
