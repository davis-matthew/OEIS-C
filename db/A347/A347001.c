/*
 * A347001  Expansion of e.g.f. exp( log(1 - x)^2 / 2 ).
 * https://oeis.org/A347001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} binomial(n-1,k-1) * |Stirling1(k,2)| * a(n-k)
 *            Initial values: a(0)=1, a(1)=0. |Stirling1(k,2)| = (k-1)! *
 *            H_{k-1} (integer). It is computed by the recurrence
 *            S1(n,2)=S1(n-1,1)+(n-1)*S1(n-1,2) with S1(1,1)=1. The
 *            implementation keeps three growing arrays: a[], fact[] and
 *            s1_2[]....
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache = NULL;      /* a(n) values */
static size_t a_len   = 0;          /* number of computed entries */
static size_t a_cap   = 0;          /* allocated capacity */

static seq_t *fact_cache = NULL;   /* factorial values fact[n] = n! */
static size_t fact_len   = 0;       /* number of computed factorials */
static size_t fact_cap   = 0;       /* allocated capacity */

static seq_t *s1_2_cache = NULL;   /* |Stirling1(n,2)| */
static size_t s1_len     = 0;       /* number of computed Stirling values */
static size_t s1_cap     = 0;       /* allocated capacity */

/* Grow a cache to hold at least 'size' elements.  If already larger, do nothing. */
static void grow_cache(idx_t size, seq_t **cache, size_t *cap, size_t *len)
{
    if (*len > 0) size = (size > *len ? size : *len);
    if (*cap >= (size_t)size) return;
    size_t new_cap = (*cap > 0 ? *cap * 2 : 1);
    if (new_cap < (size_t)size) new_cap = (size_t)size;
    seq_t *new_mem = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    if (!new_mem) { /* fall back to abort if allocation fails */
        abort();
    }
    *cache = new_mem;
    *cap   = new_cap;
}

/* Compute arrays up to index n (inclusive).  Returns 0 on success.  */
static void compute_up_to(idx_t n)
{
    /* Ensure factorial and Stirling arrays are large enough. */
    grow_cache((size_t)(n+1), &fact_cache, &fact_cap, &fact_len);
    grow_cache((size_t)(n+1), &s1_2_cache, &s1_cap, &s1_len);

    /* Compute new factorials and Stirling numbers. */
    for (idx_t i = fact_len; i <= n; ++i) {
        if (i == 0) {
            fact_cache[0] = (seq_t)1;
        } else {
            fact_cache[i] = fact_cache[i-1] * (seq_t)i;
        }

        if (i == 0 || i == 1) {
            s1_2_cache[i] = (seq_t)0;
        } else {
            /* S1(i,2) = S1(i-1,1) + (i-1)*S1(i-1,2)  with S1(i-1,1) = (i-2)!  */
            s1_2_cache[i] = fact_cache[i-2] + (seq_t)(i-1) * s1_2_cache[i-1];
        }
    }
    fact_len = (size_t)(n+1);
    s1_len   = (size_t)(n+1);

    /* Ensure a_cache is large enough. */
    grow_cache((size_t)(n+1), &a_cache, &a_cap, &a_len);

    /* Compute a values that are still missing. */
    for (idx_t i = a_len; i <= n; ++i) {
        if (i == 0) {
            a_cache[0] = (seq_t)1;          /* a(0) = 1               */
        } else if (i == 1) {
            a_cache[1] = (seq_t)0;          /* a(1) = 0               */
        } else {
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;          /* C(i-1,0) */
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = binom * s1_2_cache[k] * a_cache[i-k];
                sum += term;
                if (k < i) {
                    /* binom_{k+1} = binom_k * (i-k) / k   (exact division) */
                    binom = (binom * (seq_t)(i - k)) / (seq_t)k;
                }
            }
            a_cache[i] = sum;
        }
    }
    a_len = (size_t)(n+1);
}

/* Public sequence function */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_cache[n];
}
