/*
 * A357859  Number of integer factorizations of 2n into distinct even factors.
 * https://oeis.org/A357859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   _floor_isqrt_u64, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A357859: Number of integer factorizations of 2n into distinct even factors.
   The function computes the value by recursively enumerating distinct even
   divisors, memoising results per index. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/grow_seq_array.h"
#include "../utils/_floor_isqrt_u64.h"

/* Count factorizations of ``rem`` into distinct even factors all ≥ ``min_f``.
   The factors are enumerated in ascending order to guarantee distinctness. */
static seq_t factorizations_of(uint64_t rem, uint64_t min_f)
{
    seq_t total = 0;

    /* Trivial factorization consisting of the single factor ``rem``. */
    if ((rem & 1ULL) == 0ULL && rem >= min_f)
        total += 1;

    uint64_t limit = _floor_isqrt_u64(rem);
    for (uint64_t d = min_f; d <= limit; ++d)
    {
        if ((d & 1ULL) == 0ULL && rem % d == 0ULL)
            total += factorizations_of(rem / d, d + 1);
    }
    return total;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)    /* offset is 1 – return 0 for undefined index 0 */
        return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Grow the cache if necessary. */
    if ((size_t)n >= cache_size)
    {
        size_t old = cache_size;
        size_t needed = (size_t)n + 1;
        grow_seq_array(&cache, &cache_size, needed);
        /* Initialise new cache entries to 0. */
        for (size_t i = old; i < cache_size; ++i)
            cache[i] = 0;
    }

    /* Compute on demand, using 0 as a sentinel (no factorization can yield 0). */
    if (cache[(size_t)n] == 0)
    {
        uint64_t m = (uint64_t)n << 1;     /* m = 2 * n */
        cache[(size_t)n] = factorizations_of(m, 2);
    }

    return cache[(size_t)n];
}
