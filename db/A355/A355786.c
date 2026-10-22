/*
 * A355786  E.g.f. satisfies A(x) = 1/(1 - 2*x)^(A(x)/2).
 * https://oeis.org/A355786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * (k+1)^(k-1) * |Stirling1(n,k)|
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the sequence by building the Stirling
   numbers of the first kind row by row, storing a cache of the results.
   It keeps all intermediate values in seq_t and uses helper functions
   from the utilities for powering.  It works for any seq_t/int size
   supported by the build. */

#include <stdlib.h>
#include "../utils/pow_seq.h"

static seq_t *a_cache = NULL;          /* cache of a(n) values           */
static size_t a_cap = 0;               /* capacity of a_cache            */
static idx_t max_n_cache = 0;          /* largest n for which a has been computed */

static seq_t *pow2_cache = NULL;       /* powers of 2 up to current n    */
static size_t pow2_cap = 0;            /* capacity of pow2_cache         */

static seq_t *powkminus1 = NULL;       /* (k+1)^(k-1) for k up to current n */
static size_t powkminus1_cap = 0;      /* capacity of powkminus1 */

static seq_t *s_prev = NULL;           /* previous row of Stirling numbers */
static seq_t *s_cur  = NULL;           /* current row                      */
static size_t prev_len = 0;            /* length of s_prev (prev n + 1) */

/* forward declarations */
static void init(void);
static void ensure_pow2(idx_t n);
static void ensure_powkminus1(idx_t n);
static void ensure_stirling(idx_t n);

/* initialise static data structures on first use */
static void init(void)
{
    if (s_prev != NULL) return;        /* already initialised */

    /* a(0) = 1, a_cache size 1 */
    a_cache = (seq_t *)malloc(1 * sizeof(seq_t));
    a_cache[0] = (seq_t)1;
    a_cap = 1;
    max_n_cache = 0;

    /* Stirling numbers: row for n = 0 */
    prev_len = 1;
    s_prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
    s_prev[0] = (seq_t)1;             /* s(0,0) = 1 */

    /* powers of 2 and (k+1)^(k-1) for n = 0 */
    pow2_cache = (seq_t *)malloc(1 * sizeof(seq_t));
    pow2_cache[0] = (seq_t)1;
    pow2_cap = 1;

    powkminus1 = (seq_t *)malloc(1 * sizeof(seq_t));
    powkminus1[0] = (seq_t)1;
    powkminus1_cap = 1;
}

/* ensure pow2_cache has powers of 2 up to n */
static void ensure_pow2(idx_t n)
{
    if (pow2_cap > (size_t)n) return;
    pow2_cache = (seq_t *)realloc(pow2_cache, (n + 1) * sizeof(seq_t));
    for (idx_t i = pow2_cap; i <= n; ++i) {
        if (i == 0)
            pow2_cache[i] = (seq_t)1;
        else
            pow2_cache[i] = pow2_cache[i - 1] * (seq_t)2;
    }
    pow2_cap = n + 1;
}

/* ensure powkminus1 has (k+1)^(k-1) up to n */
static void ensure_powkminus1(idx_t n)
{
    if (powkminus1_cap > (size_t)n) return;
    powkminus1 = (seq_t *)realloc(powkminus1, (n + 1) * sizeof(seq_t));
    for (idx_t k = powkminus1_cap; k <= n; ++k) {
        if (k <= 1)
            powkminus1[k] = (seq_t)1;              /* (1)^(0) = 1 and (2)^(1-1)=1 */
        else
            powkminus1[k] = pow_seq((seq_t)(k + 1), (int)(k - 1));
    }
    powkminus1_cap = n + 1;
}

/* build Stirling numbers up to n and compute a(n) if needed */
static void ensure_stirling(idx_t n)
{
    init();                                   /* make sure everything is allocated */

    if (prev_len > (size_t)n) return;         /* already have rows up to n */

    for (idx_t m = prev_len; m <= n; ++m) {
        /* allocate current row of length m+1 */
        s_cur = (seq_t *)realloc(s_cur, (m + 1) * sizeof(seq_t));
        s_cur[0] = (seq_t)0;
        /* compute inner entries */
        for (idx_t k = 1; k < m; ++k) {
            s_cur[k] = s_prev[k - 1] + (seq_t)(m - 1) * s_prev[k];
        }
        s_cur[m] = (seq_t)1;                /* s(m,m) = 1 */

        /* compute a(m) if not already cached */
        if (max_n_cache < m) {
            ensure_pow2(m);
            ensure_powkminus1(m);
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= m; ++k) {
                seq_t term = s_cur[k];
                if (term == (seq_t)0) continue;
                term = term * pow2_cache[m - k];
                term = term * powkminus1[k];
                sum += term;
            }
            /* extend a_cache if needed */
            if (m >= (idx_t)a_cap) {
                size_t new_cap = a_cap * 2;
                if (new_cap == 0) new_cap = 8;
                while (new_cap <= (size_t)m) new_cap *= 2;
                a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
                a_cap = new_cap;
            }
            a_cache[m] = sum;
            if (m > max_n_cache) max_n_cache = m;
        }

        /* swap rows for next iteration */
        seq_t *tmp = s_prev;
        s_prev = s_cur;
        s_cur = tmp;
        prev_len = m + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n <= max_n_cache && a_cache != NULL) return a_cache[n];
    ensure_stirling(n);     /* calculates a(n) and caches up to n */
    return a_cache[n];
}
