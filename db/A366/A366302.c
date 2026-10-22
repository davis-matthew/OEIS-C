/*
 * A366302  Expansion of e.g.f. 1 / (-6 + Sum_{k=1..7} exp(-k*x)).
 * https://oeis.org/A366302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A366302 :
   a(0) = 1
   a(n) = Sum_{k=1..n} (-1)^(k+1) * binomial(n,k) * (1^k + 2^k + ... + 7^k) * a(n-k)  */
#include <stdlib.h>
#include "../utils/pow_int.h"

/* Allocate an array of seq_t of given length */
static seq_t* seq_alloc(size_t len)
{
    return (seq_t*)malloc(len * sizeof(seq_t));
}

/* Reallocate an existing array to a new length */
static seq_t* seq_realloc(seq_t* old, size_t /* old_len */, size_t new_len)
{
    return (seq_t*)realloc(old, new_len * sizeof(seq_t));
}

/* Memoisation cache for sequence values */
static seq_t* cache          = NULL;
static size_t cache_len      = 0;           /* actual allocated length */
static size_t computed_len   = 0;           /* highest index that has been computed */

/* Array for S_k = sum_{i=1..7} i^k */
static seq_t* Sk             = NULL;
static size_t sk_len         = 0;           /* actual number of stored S_k values */

/* Ensure Sk contains values up to index n (inclusive) */
static void ensure_Sk(idx_t n)
{
    if (sk_len > n) return;                /* already present */

    /* Resize Sk array */
    Sk = (idx_t)sk_len == 0 ? seq_alloc(n + 1)
                            : seq_realloc(Sk, sk_len, n + 1);

    if (sk_len == 0) {                     /* initialise S_0 = 7 */
        Sk[0] = 7;
        sk_len = 1;
    }

    /* Compute missing S_k values */
    for (idx_t k = (idx_t)sk_len; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t i = 1; i <= 7; ++i)
            sum += pow_int((seq_t)i, k);
        Sk[k] = sum;
    }
    sk_len = (size_t)(n + 1);
}

/* Compute a(n) using the recurrence, assuming smaller values are cached */
static void compute_a_n(idx_t n)
{
    ensure_Sk(n);                           /* need S_k up to k = n */

    seq_t pos = 0;                          /* sum for odd k */
    seq_t neg = 0;                          /* sum for even k */
    seq_t binom = 1;                        /* C(n,0) = 1 */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binomial coefficient C(n,k) = C(n,k-1)*(n-k+1)/k */
        binom = binom * (seq_t)(n - k + 1) / (seq_t)k;

        seq_t term = binom * Sk[k] * cache[n - k];
        if (k & 1) pos += term;
        else      neg += term;
    }
    cache[n] = pos - neg;
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 1;                   /* a(0) = 1 */

    /* Allocate cache if first call */
    if (cache_len == 0) {
        cache_len = 8;
        cache = seq_alloc(cache_len);
        cache[0] = 1;
    }

    /* Resize cache if necessary */
    if (cache_len <= (size_t)n) {
        cache_len = (size_t)n + 1;
        cache = seq_realloc(cache, cache_len, cache_len);
    }

    /* Compute missing terms sequentially, preserving memoisation */
    for (idx_t i = 1; i <= n; ++i)
        if (i > computed_len)
            compute_a_n(i), computed_len = (size_t)i;

    return cache[n];
}
