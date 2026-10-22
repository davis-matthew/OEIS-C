/*
 * A373964  Number of compositions of 6*n-4 into parts 5 and 6.
 * https://oeis.org/A373964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊(n-4)/5⌋} binomial(n+k , n-4-5k)
 *            (a(1)=a(2)=a(3)=0) This is the number of compositions of 6·n−4
 *            into parts 5 and 6. The function stores already computed values
 *            in a static cache, so any order of calls is handled efficiently.
 *            The util function `binomial(idx_t a, idx_t b)` returns...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 28, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* static cache for memoisation */
static seq_t *cache = NULL;              /* array of computed a(i) */
static size_t cache_cap = 0;             /* current capacity of array */
static idx_t computed_up_to = 0;         /* highest index already computed */

/* Ensure that cache[0..n] exists */
static void
ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                         /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term of sequence A373964 (offset = 1). */
seq_t a(idx_t n)
{
    if (n == 0)          /* below offset – undefined, return 0 */
        return 0;

    if (n <= computed_up_to)
        return cache[n];

    ensure_cache(n);

    for (idx_t i = computed_up_to + 1; i <= n; ++i)
    {
        if (i < 4)                     /* first three terms are zero */
            cache[i] = 0;
        else
        {
            seq_t sum = 0;
            idx_t kmax = (i - 4) / 5;   /* floor((i-4)/5) */
            for (idx_t k = 0; k <= kmax; ++k)
            {
                /* binomial(i+k , i-4-5k) */
                sum += binomial(i + k, i - 4 - 5 * k);
            }
            cache[i] = sum;
        }
    }

    computed_up_to = n;
    return cache[n];
}
