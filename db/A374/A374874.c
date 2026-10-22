/*
 * A374874  Obverse convolution (2^n)**(n!); see Comments.
 * https://oeis.org/A374874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0}^{n} ( 2^k + (n-k)! )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"

static seq_t *factorial_cache = NULL;
static idx_t   factorial_max  = 0;
static size_t  factorial_cap  = 0;

static void ensure_factorial(idx_t n)
{
    if (factorial_cap <= (size_t)n)
    {
        size_t new_cap = (factorial_cap < 2) ? 2 : factorial_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;

        seq_t *new_fact = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_fact) abort();

        if (factorial_cache)
        {
            memcpy(new_fact, factorial_cache,
                   (size_t)(factorial_max + 1) * sizeof(seq_t));
            free(factorial_cache);
        }

        factorial_cache = new_fact;
        factorial_cap   = new_cap;
        new_fact[0]     = 1;          /* 0! = 1 */
    }

    if (factorial_max < n)
    {
        for (idx_t i = factorial_max + 1; i <= n; ++i)
            factorial_cache[i] = factorial_cache[i - 1] * (seq_t)i;
        factorial_max = n;
    }
}

seq_t a(idx_t n)
{
    ensure_factorial(n);

    seq_t result = 1;
    for (size_t k = 0; k <= (size_t)n; ++k)
    {
        seq_t term = pow2((idx_t)k) + factorial_cache[(size_t)n - k];
        result = result * term;
    }

    return result;
}
