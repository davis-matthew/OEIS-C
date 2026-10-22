/*
 * A375381  E.g.f.: (exp(-x) - exp(x) - 2)/(exp(-x) + exp(x) - 4).
 * https://oeis.org/A375381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A375381:
   a(0) = 1
   a(n) = Σ_{j=0,2,..,n-1} binomial(n,j) * a(j)   (n > 0)
   Recurrence uses a(j) where j increments by 2.  */
#include <stdlib.h>

static seq_t *cache = NULL;   /* cached values of a(n) */
static size_t cache_size = 0; /* number of cached entries */

/* Return the nth term of A375381 */
seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return (seq_t)1;

    /* Ensure the cache is allocated and large enough */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failed */
        cache[0] = (seq_t)1;
        cache_size = 1;
    }
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;            /* initialise new slots */
        cache_size = new_size;
    }

    /* Compute values up to n that are not already cached */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0)
            continue;                       /* already computed */

        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;                /* binomial(i,0) */

        for (idx_t j = 0; j < i; j += 2) {
            /* sum += binomial(i,j) * a(j) */
            sum += bin * cache[j];

            /* Prepare binomial(i,j+2) if needed */
            if (j + 2 >= i)
                break;

            seq_t num = (seq_t)(i - j);
            num *= (seq_t)(i - j - 1);        /* (i-j)*(i-j-1) */
            seq_t den = (seq_t)(j + 1);
            den *= (seq_t)(j + 2);            /* (j+1)*(j+2) */
            bin *= num;
            bin /= den;
        }
        cache[i] = sum;
    }

    return cache[n];
}
