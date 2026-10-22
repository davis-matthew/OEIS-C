/*
 * A368319  Expansion of e.g.f. exp(2*x) / (3 - 2*exp(x)).
 * https://oeis.org/A368319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n + 2 * Sum_{k=1..n} binomial(n,k) * a(n-k). Recurrence
 *            is used with memoisation
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static storage for memoised values */
    static seq_t *cache = NULL;
    static idx_t cap = 0;      /* current capacity of cache  */
    static idx_t last = 0;     /* highest index that has been computed */
    static seq_t two_pow = (seq_t)1;   /* 2^last  */

    /* Initialise cache if first call */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out‑of‑memory; not expected in tests */
        cache[0] = (seq_t)1;
        last = 0;
        two_pow = (seq_t)1;
    }

    /* Enlarge cache if required */
    if (n + 1 > cap) {
        idx_t newcap = cap ? cap << 1 : 1;
        while (newcap <= n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out‑of‑memory */
        cap = newcap;
    }

    /* Compute values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* Update 2^i */
        two_pow = two_pow * (seq_t)2;

        /* Sum_{j=0}^{i-1} binom(i,j) * a[j] */
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;          /* binom(i,0) */
        sum += bin * cache[0];
        for (idx_t j = 1; j < i; ++j) {
            bin = bin * (seq_t)(i - j + 1);   /* bin * (i-j+1) */
            bin = bin / (seq_t)j;             /* exact division by j */
            sum += bin * cache[j];
        }

        /* a(i) = 2^i + 2 * sum */
        cache[i] = two_pow + (sum * (seq_t)2);
    }
    last = n;

    return cache[n];
}
