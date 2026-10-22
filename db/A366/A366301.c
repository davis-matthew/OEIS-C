/*
 * A366301  Expansion of e.g.f. 1 / (-5 + Sum_{k=1..6} exp(-k*x)).
 * https://oeis.org/A366301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * binomial(n,k) * (1 + 2^k + 3^k
 *            + 4^k + 5^k + 6^k) * a(n-k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache of the sequence values a[0..max] */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;          /* number of values currently stored */

/* cache of the sums S_k = 1 + 2^k + ... + 6^k, for k >= 1 */
static seq_t *s_vals = NULL;
static idx_t s_len = 0;              /* current length of s_vals */

/* powers j^k for j = 1..6, reused while computing S_k */
static seq_t pow_j[7];
static int pow_j_inited = 0;

/* ensure we have S_k for all 1 <= k <= n */
static void ensure_s_vals(idx_t n)
{
    if (s_len > n)
        return;
    /* allocate new space if needed */
    if (s_vals == NULL)
    {
        s_vals = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (s_vals == NULL)
            abort();
    }
    else
    {
        s_vals = (seq_t *)realloc(s_vals, (n + 1) * sizeof(seq_t));
        if (s_vals == NULL)
            abort();
    }

    if (!pow_j_inited)
    {
        int j;
        for (j = 1; j <= 6; ++j)
            pow_j[j] = 1;
        pow_j_inited = 1;
    }

    /* compute new S_k values */
    idx_t k;
    for (k = s_len + 1; k <= n; ++k)
    {
        seq_t sum = 0;
        int j;
        for (j = 1; j <= 6; ++j)
        {
            pow_j[j] = pow_j[j] * j;   /* multiply by j to get j^k   */
            sum = sum + pow_j[j];      /* add to the sum S_k         */
        }
        s_vals[k] = sum;
    }
    s_len = n;
}

/* ensure that cache contains a[0..n] */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    /* grow the cache if necessary */
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    if (new_cap <= (size_t)n)
        new_cap = (size_t)n + 1;
    if (cache == NULL)
    {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache == NULL)
            abort();
    }
    else
    {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (cache == NULL)
            abort();
    }
    cache_cap = new_cap;

    /* compute a[i] for all missing indices */
    idx_t i;
    for (i = cache_len; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = 1;
            continue;
        }
        seq_t pos = 0, neg = 0;
        seq_t bin = 1;  /* binomial(i,0) */
        idx_t k;
        for (k = 1; k <= i; ++k)
        {
            /* update binomial(i,k) = binomial(i,k-1)*(i-k+1)/k */
            bin = bin * (i - k + 1);
            bin = bin / k;

            seq_t term = bin;
            term = term * s_vals[k];
            term = term * cache[i - k];

            if (k & 1)   /* odd k => positive sign */
                pos = pos + term;
            else
                neg = neg + term;
        }
        cache[i] = pos - neg;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_s_vals(n);
    ensure_cache(n);
    return cache[n];
}
