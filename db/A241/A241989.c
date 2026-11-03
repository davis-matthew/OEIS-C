/*
 * A241989  Positive numbers n that are divisible by the sum of the digits of
 *          n in base 16.
 * https://oeis.org/A241989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 7603, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9250 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th positive integer divisible by the sum of its
   base‑16 digits (Harshad numbers in base 16).
   Computed by incremental search and memoization. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if x is divisible by the sum of its base‑16 digits. */
static bool is_harshad16(seq_t x)
{
    seq_t sum = 0;
    seq_t t   = x;
    while (t > 0)
    {
        sum += t & (seq_t)15;   /* add lowest hex digit */
        t >>= 4;                /* drop lowest hex digit */
    }
    return (sum != 0) && ((x % sum) == 0);
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity of cache array */
static idx_t cache_size = 0;   /* number of elements currently stored */

/* Grow the cache array to hold at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 256 : cache_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)          /* allocation failure → abort */
        abort();
    cache     = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)              /* the sequence is defined only for n ≥ 1 */
        return 0;
    if (n <= cache_size)     /* already computed */
        return cache[n - 1];

    seq_t curr = (cache_size == 0) ? (seq_t)1
                                   : cache[cache_size - 1] + 1;
    while (true)
    {
        if (is_harshad16(curr))           /* satisfies the condition */
        {
            ensure_cache(cache_size + 1);
            cache[cache_size++] = curr;   /* store new element */
            if (cache_size == n)          /* we reached the desired index */
                return curr;
        }
        curr += 1;                         /* test next integer */
    }
}
