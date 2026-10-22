/*
 * A373870  a(n) = Sum_{k=1..n} k! * k^(n-3) * |Stirling1(n,k)|.
 * https://oeis.org/A373870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k! * k^(n-3) * |Stirling1(n,k)|
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses dynamic programming to compute unsigned Stirling numbers
   of the first kind, factorials, and k^(n-3) for each n incrementally, storing
   intermediate values in static buffers so that any order of calls is supported. */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build system. */
static seq_t *a_cache = NULL;          /* Memoised sequence values. */
static size_t  a_cap   = 0;             /* Allocated size of a_cache. */
static idx_t   max_n   = 0;             /* Highest n computed so far. */

static seq_t *fact_cache = NULL;       /* Precomputed factorials. */
static size_t fact_cap   = 0;           /* Allocated size of fact_cache. */

static seq_t *st_prev = NULL;          /* Previous row of Stirling numbers. */
static seq_t *st_curr = NULL;          /* Current row of Stirling numbers. */
static size_t st_cap   = 0;             /* Allocated size for Stirling arrays. */

static seq_t *pow_k   = NULL;          /* k^(n-3) for current n. */
static size_t pow_cap = 0;             /* Allocated size for pow_k. */

static void ensure_a_cap(idx_t n)
{
    if (a_cap < (size_t)n + 1) {
        a_cap = (size_t)n + 1;
        a_cache = (seq_t *)realloc(a_cache, a_cap * sizeof(seq_t));
    }
}

static void ensure_fact_cap(idx_t n)
{
    if (fact_cap < (size_t)n + 1) {
        fact_cap = (size_t)n + 1;
        fact_cache = (seq_t *)realloc(fact_cache, fact_cap * sizeof(seq_t));
        if (fact_cache[0] == 0) fact_cache[0] = (seq_t)1;      /* Ensure 0! = 1. */
    }
}

static void ensure_st_cap(idx_t n)
{
    if (st_cap < (size_t)n + 1) {
        st_cap = (size_t)n + 1;
        st_prev = (seq_t *)realloc(st_prev, st_cap * sizeof(seq_t));
        st_curr = (seq_t *)realloc(st_curr, st_cap * sizeof(seq_t));
    }
}

static void ensure_pow_cap(idx_t n)
{
    if (pow_cap < (size_t)n + 1) {
        pow_cap = (size_t)n + 1;
        pow_k = (seq_t *)realloc(pow_k, pow_cap * sizeof(seq_t));
    }
}

static void init_cache(void)
{
    /* Initialize a(0). */
    ensure_a_cap(0);
    a_cache[0] = (seq_t)0;
    max_n = 0;

    /* Initialize factorial cache for 0!. */
    ensure_fact_cap(0);
    fact_cache[0] = (seq_t)1;

    /* Initialize Stirling arrays; nothing yet needed. */
    st_prev = NULL;
    st_curr = NULL;
    st_cap  = 0;

    /* Initialize power array. */
    pow_k   = NULL;
    pow_cap = 0;
}

seq_t a(idx_t n)
{
    /* Lazily initialize static caches on first call. */
    if (a_cache == NULL) init_cache();

    if (n <= max_n) {
        return a_cache[n];
    }

    /* Extend caches to accommodate n. */
    ensure_a_cap(n);
    ensure_fact_cap(n);
    ensure_st_cap(n);
    ensure_pow_cap(n);

    /* Compute factorials up to n. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }

    /* Compute Stirling numbers and sequence values incrementally. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* Ensure current row size. */
        ensure_st_cap(i);

        /* Compute Stirling row for n = i. */
        if (i == 1) {
            st_curr[1] = (seq_t)1;                    /* S(1,1) = 1 */
        } else {
            /* S(i,1) = (i-1)!  */
            st_curr[1] = fact_cache[i - 1];

            for (idx_t k = 2; k <= i; ++k) {
                seq_t term1 = st_prev[k - 1];         /* s(i-1,k-1)  */
                seq_t term2 = 0;
                if (k <= i - 1) {
                    term2 = st_prev[k];
                    term2 *= (seq_t)(i - 1);          /* (i-1)*s(i-1,k) */
                }
                st_curr[k] = term1 + term2;           /* s(i,k) = s(i-1,k-1)+(i-1)*s(i-1,k) */
            }
        }

        /* Compute a(i). */
        if (i == 0) {
            a_cache[0] = (seq_t)0;
        } else if (i == 1) {
            a_cache[1] = (seq_t)1;
        } else if (i == 2) {
            a_cache[2] = (seq_t)2;
        } else {
            /* Update k^(i-3) values. */
            if (i == 3) {
                for (idx_t k = 1; k <= 3; ++k) {
                    pow_k[k] = (seq_t)1;              /* k^0 = 1 */
                }
            } else {
                /* Multiply existing powers by k to increase exponent by 1. */
                for (idx_t k = 1; k <= i - 1; ++k) {
                    pow_k[k] *= (seq_t)k;
                }
                /* Compute i^(i-3) from scratch. */
                seq_t val = (seq_t)1;
                for (idx_t j = 0; j < (i - 3); ++j) {
                    val *= (seq_t)i;
                }
                pow_k[i] = val;
            }

            /* Sum over k. */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = fact_cache[k];
                term *= pow_k[k];
                term *= st_curr[k];
                sum += term;
            }
            a_cache[i] = sum;
        }

        /* Prepare for next iteration: swap rows. */
        seq_t *tmp = st_prev;
        st_prev = st_curr;
        st_curr = tmp;
    }

    max_n = n;
    return a_cache[n];
}
