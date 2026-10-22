/*
 * A367851  Expansion of e.g.f. 1/(1 - x + log(1 - 2*x)/2).
 * https://oeis.org/A367851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = n*a(n-1) + Sum_{k=1..n}
 *            2^(k-1)*(k-1)!*binomial(n,k)*a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A367851) */

#include <stdlib.h>
#include <stdint.h>

/* The types seq_t and idx_t are defined by the build system. */

static seq_t *fact_tbl   = NULL;   /* factorials: fact_tbl[i] = i!   */
static size_t fact_cap   = 0;
static seq_t *pow2_tbl   = NULL;   /* powers of two: pow2_tbl[e] = 2^e */
static size_t pow2_cap   = 0;
static seq_t *a_cache    = NULL;   /* computed a(n) values          */
static size_t a_cap      = 0;
static size_t a_len      = 0;       /* next index to compute        */

static void
ensure_factorials(idx_t n)
{
    /* ensure fact_tbl and pow2_tbl have entries up to n */
    if (fact_cap <= n) {
        size_t new_cap = n + 1;
        if (fact_tbl == NULL) {
            fact_tbl = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (fact_tbl == NULL) abort();
            fact_tbl[0] = 1;   /* 0! = 1 */
            fact_cap = 1;
        } else {
            fact_tbl = (seq_t *)realloc(fact_tbl, new_cap * sizeof(seq_t));
            if (fact_tbl == NULL) abort();
            /* fact_tbl now has previous entries */
        }
        for (size_t i = fact_cap; i <= n; ++i) {
            fact_tbl[i] = fact_tbl[i - 1] * (seq_t)i;
        }
        fact_cap = new_cap;
    }

    if (pow2_cap <= n) {
        size_t new_cap = n + 1;
        if (pow2_tbl == NULL) {
            pow2_tbl = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (pow2_tbl == NULL) abort();
            pow2_tbl[0] = 1;   /* 2^0 = 1 */
            pow2_cap = 1;
        } else {
            pow2_tbl = (seq_t *)realloc(pow2_tbl, new_cap * sizeof(seq_t));
            if (pow2_tbl == NULL) abort();
        }
        for (size_t i = pow2_cap; i <= n; ++i) {
            pow2_tbl[i] = pow2_tbl[i - 1] * 2;
        }
        pow2_cap = new_cap;
    }
}

static void
ensure_a_cache(idx_t n)
{
    /* ensure a_cache can hold values up to n */
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (a_cache == NULL) abort();
        a_cache[0] = 1;   /* a(0) = 1 */
        a_len   = 1;
        a_cap   = n + 1;
    } else if (a_cap <= n) {
        a_cache = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
        if (a_cache == NULL) abort();
        a_cap = n + 1;
    }
    /* compute missing terms */
    for (idx_t i = a_len; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;  /* binomial(i,0) */
        for (idx_t k = 1; k <= i; ++k) {
            /* binom(i,k) = binom(i,k-1) * (i-k+1) / k */
            binom = (binom * (seq_t)(i - k + 1)) / (seq_t)k;
            seq_t t = binom * a_cache[i - k];
            t *= pow2_tbl[k - 1];     /* 2^(k-1) */
            t *= fact_tbl[k - 1];     /* (k-1)!   */
            sum += t;
        }
        a_cache[i] = ((seq_t)i) * a_cache[i - 1] + sum;
        a_len = i + 1;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_factorials(n);   /* we need (k-1)! and 2^(k-1) up to n-1 */
    ensure_a_cache(n);      /* compute a(n) if not yet cached */
    return a_cache[n];
}
