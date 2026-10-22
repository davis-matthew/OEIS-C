/*
 * A350759  a(n) = Sum_{k=0..n} (-1)^k*A345652(k)*Stirling1(n, k).
 * https://oeis.org/A350759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n-2} (n-2-k)! * binomial(n-1, k) * (-1)^(n-1-k)
 *            * a(k) with a(0) = 1, a(1) = 0. The implementation memoizes all
 *            a(k) up to the largest n seen and uses an auxiliary factorial
 *            cache for factorials up to n-2. All arithmetic is performed with
 *            seq_t. realloc/calloc cast to seq_t* for...
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 15, i64 n <= 22, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* cache for a(n) values */
static seq_t *a_cache = NULL;      /* array of a(n) */
static size_t a_cap   = 0;         /* capacity of a_cache */
static idx_t  a_max   = (idx_t)-1; /* largest n computed, -1 means none */

/* cache for factorials */
static seq_t *fact_cache = NULL;   /* array of factorials */
static size_t fact_cap   = 0;      /* capacity of fact_cache */
static idx_t  fact_max   = (idx_t)-1; /* largest factorial computed */

/* Ensure a_cache has space for at least (n+1) entries. */
static void ensure_a(idx_t n)
{
    if ((idx_t)a_cap <= n) {
        size_t new_cap = a_cap ? a_cap * 2 : 64;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache = tmp;
        a_cap   = new_cap;
    }
}

/* Ensure factorial cache holds factorials up to 'upto'. */
static void ensure_fact(idx_t upto)
{
    if (fact_cache == NULL) {
        size_t init_cap = upto + 1;
        if (init_cap < 64) init_cap = 64;
        fact_cache = (seq_t *)calloc(init_cap, sizeof(seq_t));
        if (!fact_cache) abort();          /* safety */
        fact_cache[0] = (seq_t)1;
        fact_cache[1] = (seq_t)1;
        fact_cap = init_cap;
        fact_max = 1;
    }
    if ((idx_t)fact_cap <= upto) {
        size_t new_cap = fact_cap ? fact_cap * 2 : 64;
        if (new_cap <= (size_t)upto) new_cap = (size_t)upto + 1;
        seq_t *tmp = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
        fact_cache = tmp;
        fact_cap   = new_cap;
    }
    for (idx_t i = fact_max + 1; i <= upto; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_max = upto;
}

/* Function to compute a(n). */
seq_t a(idx_t n)
{
    /* Handle base case a(0) = 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* On first call, initialise caches and store a(0). */
    if (a_cache == NULL) {
        ensure_a(0);
        a_cache[0] = (seq_t)1;
        a_max = 0;
    }

    /* Ensure we can write a(n). */
    ensure_a(n);

    /* Precompute factorials up to n-2 if needed. */
    if (n >= 2)
        ensure_fact(n - 2);

    /* Compute missing a values up to n. */
    for (idx_t cur = a_max + 1; cur <= n; ++cur) {
        seq_t sum = 0;
        /* For cur == 0 or 1, the inner loop is empty, sum stays 0 or 1. */
        if (cur >= 2) {
            seq_t binom = 1;
            for (idx_t k = 0; k <= cur - 2; ++k) {
                /* sign = (-1)^(cur-1-k) : negative when (cur-1-k) is odd. */
                bool neg = ((cur - 1 - k) & 1) != 0;

                seq_t term = fact_cache[cur - 2 - k];  /* (cur-2-k)! */
                term *= binom;                         /* * C(cur-1,k) */
                term *= a_cache[k];                    /* * a(k) */
                if (neg) term = -term;
                sum += term;

                /* Update binomial for next k: C(cur-1,k+1) */
                if (k + 1 <= cur - 2) {
                    binom = binom * (seq_t)(cur - 1 - k) / (seq_t)(k + 1);
                }
            }
        } else if (cur == 1) {
            /* sum remains 0 because no terms; matches a(1)=0 */
        } else { /* cur == 0 handled earlier */
            sum = (seq_t)1;
        }
        a_cache[cur] = sum;
    }

    a_max = n;
    return a_cache[n];
}
