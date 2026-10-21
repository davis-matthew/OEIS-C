/*
 * A241479  GCD of the solutions x of sigma(x) = sigma(n), where sigma(n) =
 *          A000203(n) = sum of divisors of n.
 * https://oeis.org/A241479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = GCD of solutions x such that sigma(x) = sigma(n)
 * Range:     u8 n <= 254, u16 n <= 9028, u32 n <= 9028, u64 n <= 9028,
 *            u128 n <= 9028, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

static seq_t *sigma_cache = NULL;
static idx_t sigma_cap = 0;

static void extend_sigma_cache(idx_t upto)
{
    if (sigma_cap >= upto)
        return;
    idx_t new_cap = sigma_cap > 0 ? sigma_cap : 1;
    while (new_cap < upto)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    if (!new_cache)
        abort();
    for (idx_t i = 0; i <= new_cap; ++i)
        new_cache[i] = (seq_t)1;
    new_cache[0] = (seq_t)0;
    for (idx_t i = 2; i <= new_cap; ++i)
    {
        for (idx_t j = i; j <= new_cap; j += i)
            new_cache[j] = new_cache[j] + (seq_t)i;
    }
    free(sigma_cache);
    sigma_cache = new_cache;
    sigma_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* offset is 1, but guard against 0 */

    seq_t S = sigma_of(n);
    if (S <= (seq_t)1)
        return (seq_t)1;  /* n == 1 */

    idx_t limit = (idx_t)S; /* safe cast for n <= 10000 */
    extend_sigma_cache(limit);

    seq_t g = (seq_t)0;
    for (idx_t x = 1; x < (idx_t)S; ++x)
    {
        if (sigma_cache[x] == S)
        {
            if (g == (seq_t)0)
                g = (seq_t)x;
            else
                g = gcd(g, (seq_t)x);
        }
    }
    /* For n > 1, n itself will be included in the loop above. */
    if (g == (seq_t)0)
        return (seq_t)n;
    return g;
}
