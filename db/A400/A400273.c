/*
 * A400273  G.f. A(x) satisfies A(x) = (1 + x) * A(x^4 * (1 + x)^3), with A(0)
 *          = 1.
 * https://oeis.org/A400273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 26, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A400273
   Formula: a(0) = 1;
   a(n) = sum_{k=0..⌊n/4⌋} binomial(3*k+1, n-4*k) * a(k).   */

/*  The cache is grown as needed; it stores all previously computed
    values of a up to the highest index requested.  The code works
    for both C11 and C++17, where seq_t may be an arbitrary-precision
    integer class or a fixed-width unsigned integer type. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;     /* array holding already computed a(n) */
static size_t cache_cap = 0;    /* allocated capacity of cache */
static idx_t  cache_last = 0;   /* highest index whose value is stored */
static bool    cache_init = false;   /* true once a(0) is set */

/* Ensure cache can hold indices 0..n (inclusive).  The allocation
   uses a power-of-two growth for amortized linear time.  The cast
   from void* to seq_t* is required for C++ compilers. */
static void grow_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();     /* unrecoverable memory error */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute and store a(m) for all m from cache_last+1 up to n inclusive. */
static void compute_to(idx_t n)
{
    grow_cache(n);
    for (idx_t i = cache_last + 1; i <= n; ++i)
    {
        seq_t sum = 0;
        idx_t max_k = i / 4;           /* floor(i/4) */
        for (idx_t k = 0; k <= max_k; ++k)
        {
            seq_t coeff = binomial(3 * k + 1, i - 4 * k);
            sum += coeff * cache[k];
        }
        cache[i] = sum;
    }
    cache_last = n;
}

extern seq_t a(idx_t n)
{
    /* Initialise base case a(0)=1 on first call. */
    if (!cache_init)
    {
        grow_cache(0);
        cache[0] = (seq_t)1;
        cache_last = 0;
        cache_init = true;
    }

    if (n > cache_last)
        compute_to(n);

    return cache[n];
}
