/*
 * A355284  Expansion of e.g.f. 1 / (1 + x + x^2/2 + log(1 - x)).
 * https://oeis.org/A355284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=0, a(n)=Sum_{k=3..n}
 *            binomial(n,k)*(k-1)!*a(n-k) (OEIS A355284)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cache for a(n) values */
static seq_t *a_cache = NULL;
static size_t a_cap = 0;
static idx_t a_last = (idx_t)-1;

/* Cache for factorials */
static seq_t *fact_cache = NULL;
static size_t fact_cap = 0;
static idx_t fact_last = (idx_t)-1;

/* Ensure factorial cache up to n (computes factorial[0..n]) */
static void ensure_fact_cap(idx_t n)
{
    if (fact_cache == NULL) {
        size_t sz = n + 1;
        if (sz < 4) sz = 4;
        fact_cache = (seq_t*)malloc(sz * sizeof(seq_t));
        fact_cap   = sz;
        fact_last  = 0;
        fact_cache[0] = 1;
    } else if (fact_cap <= (size_t)n) {
        size_t new_cap = fact_cap * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *new_fact = (seq_t*)malloc(new_cap * sizeof(seq_t));
        for (size_t i = 0; i < fact_cap; ++i) new_fact[i] = fact_cache[i];
        free(fact_cache);
        fact_cache = new_fact;
        fact_cap   = new_cap;
    }
    if (fact_last < n) {
        for (idx_t i = fact_last + 1; i <= n; ++i) {
            if (i == 0) fact_cache[i] = 1;
            else fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_last = n;
    }
}

/* Ensure a(n) cache up to n (computes a[0..n]) */
static void ensure_a_cache(idx_t n)
{
    if (a_cache == NULL) {
        size_t sz = n + 1;
        if (sz < 4) sz = 4;
        a_cache = (seq_t*)malloc(sz * sizeof(seq_t));
        a_cap   = sz;
    } else if (a_cap <= (size_t)n) {
        size_t new_cap = a_cap * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *new_a = (seq_t*)malloc(new_cap * sizeof(seq_t));
        for (size_t i = 0; i < a_cap; ++i) new_a[i] = a_cache[i];
        free(a_cache);
        a_cache = new_a;
        a_cap   = new_cap;
    }

    /* Initialize base values if needed */
    if (a_last == (idx_t)-1) {
        a_cache[0] = 1;
        a_last = 0;
        if (n >= 1) { a_cache[1] = 0; a_last = 1; }
        if (n >= 2) { a_cache[2] = 0; a_last = 2; }
    }

    for (idx_t i = a_last + 1; i <= n; ++i) {
        ensure_fact_cap(i - 1);          /* need facts up to (i-1)! */
        seq_t sum = 0;
        seq_t binom = 1;

        for (idx_t k = 1; k <= i; ++k) {
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k;
            if (k < 3) continue;
            seq_t term = binom * fact_cache[k - 1];
            term = term * a_cache[i - k];
            sum += term;
        }
        a_cache[i] = sum;
        a_last = i;
    }
}

/* Public function to obtain a(n) */
seq_t a(idx_t n)
{
    ensure_a_cache(n);
    return a_cache[n];
}
