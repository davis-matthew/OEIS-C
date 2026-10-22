/*
 * A400577  a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(n-1,k) * k! * a(k).
 * https://oeis.org/A400577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, new
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A400577:
   a(0) = 1;
   a(n) = Σ_{k=0}^{n-1} binomial(n-1,k) * k! * a(k)   for n ≥ 1
*/

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free, abort */

static seq_t *fact = NULL;          /* fact[i] = i! */
static size_t fact_len = 0;         /* number of elements stored (0..fact_len-1) */

static seq_t *cache = NULL;        /* cache[i] = a(i) */
static size_t cache_len = 0;       /* number of elements stored (0..cache_len-1) */

/* Ensure that fact contains factorials up to n (inclusive). */
static void
ensure_fact_upto(idx_t n)
{
    if (fact_len > (size_t)n)
        return;                       /* already available */

    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(fact, new_len * sizeof(seq_t));
    if (!tmp) abort();
    fact = tmp;
    /* initialise from previous length to n */
    idx_t start = (idx_t)fact_len;     /* first new index */
    if (fact_len == 0)                 /* missing a[0] = 1 */
    {
        fact[0] = (seq_t)1;
        start = 1;
    }
    for (idx_t i = start; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;
    fact_len = new_len;
}

/* Ensure that cache contains values up to n (inclusive). */
static void
ensure_cache_upto(idx_t n)
{
    if (cache_len > (size_t)n)
        return;                       /* already available */

    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute missing entries */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)1;
            continue;
        }

        /* need factorials up to i-1 */
        ensure_fact_upto(i - 1);
        seq_t sum = 0;
        seq_t fact_i_1 = fact[i - 1];       /* (i-1)! */

        for (idx_t k = 0; k < i; ++k)
        {
            /* fact_i_1 / fact[i-1-k] = (i-1)! / (i-1-k)! */
            seq_t term = fact_i_1 / fact[i - 1 - k];
            term = term * cache[k];
            sum += term;
        }
        cache[i] = sum;
    }

    cache_len = new_len;
}

/* PUBLIC FUNCTION */
seq_t
a(idx_t n)
{
    ensure_cache_upto(n);
    return cache[(size_t)n];
}
