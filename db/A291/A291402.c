/*
 * A291402  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - S^3 - S^4.
 * https://oeis.org/A291402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-3) + 4*a(n-4) + 7*a(n-5) + 7*a(n-6)
   + 4*a(n-7) + a(n-8)  for n ≥ 9.
   Initial values:
     a(0)=0, a(1)=0, a(2)=1, a(3)=4,
     a(4)=7, a(5)=8, a(6)=12, a(7)=31, a(8)=71. */
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;          /* cached sequence values      */
static size_t capacity = 0;          /* number of elements allocated */
static size_t computed = 0;          /* terms that have been filled   */

/* Ensure that the cache can address indices up to n. */
static void ensure_capacity(idx_t n)
{
    if (capacity > (size_t)n)
        return;

    size_t new_cap = capacity ? capacity : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *old = cache;
    cache = seq_alloc_new_array(new_cap);

    /* Copy previous values and zero the rest. */
    for (size_t i = 0; i < capacity; ++i)
        cache[i] = old[i];
    for (size_t i = capacity; i < new_cap; ++i)
        cache[i] = 0;

    capacity = new_cap;
    if (old)
        seq_free_array(old);
}

/* Initialise the first nine terms. */
static void init_base(void)
{
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)4;
    cache[4] = (seq_t)7;
    cache[5] = (seq_t)8;
    cache[6] = (seq_t)12;
    cache[7] = (seq_t)31;
    cache[8] = (seq_t)71;
    computed = 9;   /* indices 0 … 8 are now stored */
}

/* Compute a(n) using memoisation. */
seq_t a(idx_t n)
{
    if (!cache) {
        ensure_capacity(8);   /* allocate at least indices 0…8 */
        init_base();
    }

    /* Return cached value if it already exists. */
    if ((size_t)n < computed)
        return cache[(size_t)n];

    /* Make sure the cache is large enough. */
    ensure_capacity(n);

    /* Compute missing terms up to n. */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        seq_t val = cache[i-3];
        val += cache[i-4] * (seq_t)4;
        val += cache[i-5] * (seq_t)7;
        val += cache[i-6] * (seq_t)7;
        val += cache[i-7] * (seq_t)4;
        val += cache[i-8];
        cache[i] = val;
    }
    computed = (size_t)n + 1;
    return cache[(size_t)n];
}
