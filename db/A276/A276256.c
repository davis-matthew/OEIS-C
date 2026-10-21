/*
 * A276256  A nonlinear recurrence of order 3: a(1)=a(2)=a(3)=1;
 *          a(n)=(a(n-1)+a(n-2)+1)^2/a(n-3).
 * https://oeis.org/A276256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((a(n-1)+a(n-2)+1)^2)/a(n-3); a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#if defined(__cplusplus)
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n <= 3) return (seq_t)1;

#if defined(__cplusplus)
    /* C++ implementation using std::vector for arbitrary‑precision types. */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(4);            /* cache[0] unused, cache[1..3] = 1 */
        cache[1] = cache[2] = cache[3] = (seq_t)1;
    }

    /* Grow the cache to size n+1 if necessary. */
    if (n >= cache.size()) {
        size_t old = cache.size();
        cache.resize(static_cast<size_t>(n) + 1);
        for (size_t i = old; i <= static_cast<size_t>(n); ++i) {
            seq_t sum = cache[i-1] + cache[i-2] + (seq_t)1;
            cache[i] = (sum * sum) / cache[i-3];
        }
    }

    return cache[n];
#else
    /* C implementation using malloc/realloc for primitive types. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;           /* actual size of allocated array */

    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failure */
        cache[1] = cache[2] = cache[3] = (seq_t)1;
        cache_size = 4;
    }

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = cache[i-1] + cache[i-2] + (seq_t)1;
            cache[i] = (sum * sum) / cache[i-3];
        }
        cache_size = new_size;
    }

    return cache[n];
#endif
}
