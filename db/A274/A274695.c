/*
 * A274695  a(1) = 1; for n>1, a(n) = smallest number > a(n-1) such that
 *          a(1)*a(2)*...*a(n) + 1 is a Fibonacci number.
 * https://oeis.org/A274695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A274695: a(1)=1, a(n)=smallest integer >a(n-1) such that
   a(1)*a(2)*…*a(n)+1 is a Fibonacci number.
*/

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Cached terms, their product and the current Fibonacci state. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;  /* how many terms are stored          */
static size_t cache_cap  = 0;  /* allocated capacity of cache        */
static seq_t prod        = 0;  /* product of all cached terms        */

static seq_t fib_prev = (seq_t)1;  /* F(k-1)  */
static seq_t fib_curr = (seq_t)1;  /* F(k)    */

/* Grow the cache to at least 'need' entries.  This function
   works for both native integer and arbitrary‑precision seq_t. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the first two terms: a(1)=1, a(2)=2. */
static void init_cache(void)
{
    if (cache_len > 0)         /* already initialised */
        return;

    grow_cache(2);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)2;
    cache_len = 2;
    prod = cache[0] * cache[1]; /* product of first two terms           */

    /* Fibonacci seed: F(1)=1, F(2)=1. */
    fib_prev = (seq_t)1;        /* F(1)   */
    fib_curr = (seq_t)1;        /* F(2)   */
}

/* Return the nth term of the sequence.  the function may be
   called in any order. */
seq_t a(idx_t n)
{
    if (n < 1)          /* undefined behaviour for n=0 */
        return (seq_t)0;

    init_cache();       /* ensure cache is set up */

    /* Return a cached value if already computed. */
    if ((idx_t)cache_len >= n)
        return cache[n - 1];

    /* Generate terms up to the requested index. */
    while ((idx_t)cache_len < n)
    {
        const seq_t prev = cache[cache_len - 1];   /* a(n-1) */

        /* Search for the next Fibonacci number that yields
           a valid term: F_k ≡ 1 (mod prod). */
        for (;;)
        {
            /* Compute next Fibonacci number: F_k = fib_prev + fib_curr. */
            seq_t next_f = fib_prev + fib_curr;
            fib_prev = fib_curr;
            fib_curr = next_f;

            /* F_k must be larger than the current product. */
            if (fib_curr <= prod)
                continue;

            /* Check divisibility: (F_k - 1) must be a multiple of prod. */
            seq_t diff = fib_curr - (seq_t)1;
            if (diff % prod != 0)
                continue;

            /* Candidate term. */
            seq_t candidate = diff / prod;
            if (candidate <= prev)
                continue;          /* must be strictly larger than a(n-1) */

            /* Store the new term and update the product. */
            grow_cache(cache_len + 1);
            cache[cache_len] = candidate;
            prod *= candidate;     /* update product for next iteration */
            cache_len++;
            break;                 /* move on to the next n */
        }
    }

    return cache[n - 1];
}
