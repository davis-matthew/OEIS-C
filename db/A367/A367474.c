/*
 * A367474  Expansion of e.g.f. 1 / (1 + 2 * log(1 - x))^2.
 * https://oeis.org/A367474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * (k+1)! * |S1(n,k)| where S1(n,k) are
 *            unsigned Stirling numbers of the first kind. The recurrence for
 *            |S1| is |S1(0,0)| = 1, |S1(n,0)| = 0 (n>0) |S1(n,k)| =
 *            |S1(n-1,k-1)| + (n-1) * |S1(n-1,k)| (1 <= k <= n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --------- helper arrays ------------------------------------------- */

static seq_t *st_prev = NULL;   /* previous Stirling row  (size = m) */
static seq_t *st_cur  = NULL;   /* current  Stirling row  (size = m+1) */
static size_t stir_cap = 0;     /* current capacity of stirling arrays */

static seq_t *pow2 = NULL;      /* powers of 2  (size >= n+1) */
static size_t pow2_cap = 0;

static seq_t *fact = NULL;      /* factorials  (size >= n+2) */
static size_t fact_cap = 0;

/* -------------------------------------------------------------------- */

static void ensure_stirling(idx_t n)
{
    /* need capacity n+1 (for current row of size n+1) */
    if ((size_t)n + 1 > stir_cap) {
        size_t new_cap = stir_cap ? stir_cap * 2 : 16;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        st_prev = (seq_t *)realloc(st_prev, new_cap * sizeof(seq_t));
        st_cur  = (seq_t *)realloc(st_cur,  new_cap * sizeof(seq_t));
        stir_cap = new_cap;
    }
}

static void ensure_pow2(idx_t n)
{
    /* need capacity n+1 */
    if ((size_t)n + 1 > pow2_cap) {
        size_t new_cap = pow2_cap ? pow2_cap * 2 : 16;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        pow2 = (seq_t *)realloc(pow2, new_cap * sizeof(seq_t));
        pow2_cap = new_cap;
        /* fill new entries */
        for (size_t i = (pow2_cap >> 1); i < pow2_cap; ++i)
            pow2[i] = pow2[i-1] * (seq_t)2;
    }
}

static void ensure_fact(idx_t n)
{
    /* need capacity n+2 (for factorial(n+1)) */
    if ((size_t)n + 2 > fact_cap) {
        size_t new_cap = fact_cap ? fact_cap * 2 : 16;
        while ((size_t)n + 2 > new_cap) new_cap <<= 1;
        fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        fact_cap = new_cap;
        /* fill new entries */
        for (size_t i = (fact_cap >> 1); i < fact_cap; ++i)
            fact[i] = fact[i-1] * (seq_t)i;
    }
}

/* -------------------------------------------------------------------- */

static seq_t *cache = NULL;   /* cached a(n) values */
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t last_computed = (idx_t)-1;   /* last n for which we built up to */

/* -------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Special case n==0 */
    if (n == 0) return (seq_t)1;

    /* Initialise static tables on first call */
    if (last_computed == (idx_t)-1) {
        /* Stirling arrays */
        stir_cap = 16;
        st_prev = (seq_t *)malloc(stir_cap * sizeof(seq_t));
        st_cur  = (seq_t *)malloc(stir_cap * sizeof(seq_t));
        st_prev[0] = 1;   /* |S1(0,0)| = 1 */
        /* Power of 2 */
        pow2_cap = 16;
        pow2    = (seq_t *)malloc(pow2_cap * sizeof(seq_t));
        pow2[0] = (seq_t)1;
        for (size_t i = 1; i < pow2_cap; ++i) pow2[i] = pow2[i-1] * (seq_t)2;
        /* Factorials */
        fact_cap = 16;
        fact    = (seq_t *)malloc(fact_cap * sizeof(seq_t));
        fact[0] = (seq_t)1;
        fact[1] = (seq_t)1;   /* 1! */
        for (size_t i = 2; i < fact_cap; ++i) fact[i] = fact[i-1] * (seq_t)i;

        /* Cache for sequence values */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 1;        /* we will store a(0) but will override it below */
        cache[0] = (seq_t)1;  /* a(0) */
        last_computed = 0;
    }

    /* Resize cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Return cached value if available */
    if (n < cache_len)
        return cache[n];

    /* Compute values from last_computed+1 up to n */
    for (idx_t m = last_computed + 1; m <= n; ++m) {
        ensure_stirling(m);
        ensure_pow2(m);
        ensure_fact(m);

        /* --- compute Stirling row st_cur for n=m ------------------ */
        st_cur[0] = 0;  /* |S1(m,0)| = 0 for m>0 */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term1 = (k-1 <= (idx_t)(m-1)) ? st_prev[k-1] : 0;
            seq_t term2 = st_prev[k] * (seq_t)(m-1); /* (m-1)*S1(m-1,k) */
            st_cur[k] = term1 + term2;
        }

        /* --- compute a(m) ---------------------------------------- */
        seq_t sum = 0;
        for (idx_t k = 0; k <= m; ++k) {
            seq_t term = pow2[k];
            term *= fact[k+1];
            term *= st_cur[k];
            sum += term;
        }

        cache[m] = sum;
        if (m >= (idx_t)cache_len)
            cache_len = (size_t)m + 1;

        /* swap rows for next iteration */
        seq_t *tmp = st_prev;
        st_prev = st_cur;
        st_cur  = tmp;
    }

    last_computed = n;
    return cache[n];
}
