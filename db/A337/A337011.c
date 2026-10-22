/*
 * A337011  a(n) = 2^n * exp(-1/2) * Sum_{k>=0} (k + 2)^n / (2^k * k!).
 * https://oeis.org/A337011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A337011
   Recurrence (from OEIS entry):
     a(0) = 1
     a(1) = 5
     a(n) = 5*a(n-1) + Σ_{k=2..n} binomial(n-1, k-1)*2^(k-1)*a(n-k)
   (alternatively a(n) = 2^n exp(−½) ∑_{k≥0} (k+2)^n/(2^k k!). */
#include <stdlib.h>

static seq_t *cache = NULL;     /* Cached values of a(n) */
static size_t cache_cap = 0;    /* Allocated size of cache array   */
static idx_t cache_size = 0;    /* Number of values actually stored */

seq_t a(idx_t n)
{
    /* 'a(0)' is the first term of the sequence */
    if (n == 0) {
        return (seq_t)1;   /* no caching needed for this trivial case */
    }

    /* Initialise cache if this is the first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;               /* a(0) */
        cache[1] = (seq_t)5;               /* a(1) */
        cache_size = 2;
    }

    /* Grow the cache so that it can hold a(n) */
    while ((idx_t)(cache_cap) <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) abort();            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)5;
        } else {
            /* 5*a(i-1) */
            seq_t term = (seq_t)5 * cache[i - 1];
            seq_t sum = 0;

            /* Sum over k = 2 .. i */
            seq_t binom = (seq_t)(i - 1);   /* binomial(i-1, 1) */
            seq_t pow2  = (seq_t)2;         /* 2^(k-1) for k=2 */

            for (idx_t k = 2; k <= i; ++k) {
                sum += binom * pow2 * cache[i - k];

                if (k < i) {
                    /* Prepare for the next k:
                       binom_{k+1} = binom_k * (i - k) / k
                       (this keeps binomial(i-1, k) exact)            */
                    binom = (binom * (seq_t)(i - k)) / (seq_t)k;
                    pow2 <<= 1;           /* multiply by 2 */
                }
            }
            val = term + sum;
        }
        cache[i] = val;
    }

    cache_size = n + 1;
    return cache[n];
}
