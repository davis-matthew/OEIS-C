/*
 * A352358  Expansion of e.g.f. 1/(1 - Sum_{k>=1} binomial(k+3,4) * x^k/k!).
 * https://oeis.org/A352358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence OEIS A352358
 * Recurrence: a(0)=1
 *             a(n)=∑_{k=1..n} binomial(k+3,4)·binomial(n,k)·a(n‑k)
 * The implementation uses memoisation and keeps the auxiliary tables
 * packed for all indices requested so far.
 */

#include <stdlib.h>
#include <stdint.h>

/*---  static storage for the sequence -----------------------------------*/
static seq_t *a_cache = NULL;  /* a_cache[n] = a(n) for computed indices */
static size_t a_cap   = 0;      /* current allocation limit of a_cache  */
static idx_t a_size  = 0;       /* next index that has not yet been filled */

/*---  static storage for binomial(k+3,4) ---------------------------------*/
static seq_t *bin4_cache = NULL;  /* bin4_cache[k] = binomial(k+3,4)  */
static size_t bin4_cap   = 0;      /* allocated capacity */
static size_t bin4_used  = 0;      /* number of entries that have a value */

/*---  utility: grow a seq_t array ---------------------------------------*/
static void ensure_capacity(seq_t **ptr, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap ? *cap * 2 : 1);
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    if (!tmp && new_cap != 0) abort();          /* unrecoverable */
    *ptr = tmp;
    *cap = new_cap;
}

/*---  fill bin4_cache up to index n ------------------------------------*/
static void ensure_bin4_cache(idx_t n)
{
    /* make sure space exists */
    ensure_capacity(&bin4_cache, &bin4_cap, (size_t)n + 1);

    /* compute missing entries */
    for (size_t k = bin4_used; k <= (size_t)n; ++k)
    {
        seq_t v = (seq_t)(k + 3);
        v = v * (seq_t)(k + 2);
        v = v * (seq_t)(k + 1);
        v = v * (seq_t)k;
        v = v / (seq_t)24;            /* binomial(k+3,4) */
        bin4_cache[k] = v;
    }
    bin4_used = (size_t)n + 1;
}

/*---  ensure a_cache has room for indices up to n ---------------------*/
static void ensure_a_cache(idx_t n)
{
    ensure_capacity(&a_cache, &a_cap, (size_t)n + 1);
    if (a_size == 0)                /* initialise a(0) */
    {
        a_cache[0] = (seq_t)1;      /* a(0) = 1 */
        a_size = 1;
    }
}

/*---  compute all a(i) for   a_size ≤ i ≤ n ----------------------------*/
static void compute_up_to(idx_t n)
{
    for (idx_t i = a_size; i <= n; ++i)
    {
        /* every batch needs binomial(k+3,4) up to k=i */
        ensure_bin4_cache(i);

        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;        /* binom(i,0) */
        for (idx_t k = 1; k <= i; ++k)
        {
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k;   /* binom(i,k) */
            seq_t term = bin4_cache[k] * binom * a_cache[i - k];
            sum += term;
        }
        a_cache[i] = sum;
    }
    a_size = n + 1;
}

/*---  public function ----------------------------------------------------*/
seq_t a(idx_t n)
{
    /* protect against the unlikely case of n > SIZE_MAX; such a call is
       outside the valid domain of the sequence, so we simply return 0.   */
    if ((size_t)n > SIZE_MAX) return (seq_t)0;

    ensure_a_cache(n);
    compute_up_to(n);
    return a_cache[(size_t)n];
}
