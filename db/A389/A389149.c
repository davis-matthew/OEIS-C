/*
 * A389149  Expansion of e.g.f. exp(x^3/6 * cosh(x)).
 * https://oeis.org/A389149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n)=Σ_{k odd≥3} C(n-1,k-1)*k*(k-1)*(k-2)/6 * a(n-k) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *a_cache = NULL;
static size_t cache_size = 0;
static idx_t computed_max = 0;

/* Ensure that the cache has room for indices up to n. */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) > cache_size)
    {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_arr = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!new_arr)
            exit(1); /* Out of memory */
        a_cache = new_arr;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    /* Allocate space for the requested index if needed. */
    ensure_cache(n);

    /* Initialize base values if not already done. */
    if (computed_max < 2)
    {
        if (cache_size < 3)
            ensure_cache(2);
        a_cache[0] = 1;
        a_cache[1] = 0;
        a_cache[2] = 0;
        computed_max = 2;
    }

    /* Compute new values up to n. */
    if (n > computed_max)
    {
        for (idx_t i = computed_max + 1; i <= n; ++i)
        {
            seq_t sum = 0;
            seq_t binom = 1; /* C(i-1,0) */

            for (idx_t k = 1; k <= i; ++k)
            {
                if (k > 1)
                {
                    binom = binom * (seq_t)(i - k + 1) / (seq_t)(k - 1);
                }

                if (k >= 3 && ((k & 1) != 0))
                {
                    /* Compute p_k = k*(k-1)*(k-2)/6 */
                    seq_t pk = (seq_t)k * (k - 1) * (k - 2) / 6;
                    sum += binom * pk * a_cache[i - k];
                }
            }
            a_cache[i] = sum;
        }
        computed_max = n;
    }

    return a_cache[n];
}
