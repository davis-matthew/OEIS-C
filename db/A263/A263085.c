/*
 * A263085  Partial sums of A099774 (A099774(n) = number of divisors of n-th
 *          odd number).
 * https://oeis.org/A263085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 * a(n) = a(n-1) + A000005(2n-1) for n > 1 * where
 *            A000005(m) is the number of divisors of m
 * Range:     u8 n <= 77, u16 n <= 8644, u32 n <= 8644, u64 n <= 8644,
 *            u128 n <= 8644, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Computes the divisor count of a 64‑bit integer.  */
static inline seq_t divisor_count_u64(uint64_t n)
{
    seq_t cnt = 0;
    for (uint64_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
            cnt += (d * d == n) ? 1 : 2;
    }
    return cnt;
}

/* Cached partial sums.  cache[0] is unused (dummy 0).  */
static seq_t *cache   = NULL;   /* pointer to the array */
static size_t cache_len = 0;    /* highest index currently stored (>=0) */

/* External linkage required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* value for out‑of‑range index, not used */

    /* Expand cache if needed. */
    if (n > cache_len)
    {
        size_t new_len = (size_t)n;
        seq_t *tmp = (cache_len == 0)
                     ? (seq_t *)malloc((new_len + 1) * sizeof(seq_t))
                     : (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
        if (tmp == NULL)
            abort();            /* memory allocation failed */

        cache   = tmp;
        cache[0] = 0;           /* dummy zero for easier indexing */

        for (size_t i = cache_len + 1; i <= new_len; ++i)
        {
            uint64_t odd = 2ULL * (uint64_t)i - 1ULL;
            seq_t d = divisor_count_u64(odd);
            cache[i] = cache[i - 1] + d;
        }
        cache_len = new_len;
    }

    return cache[n];
}
