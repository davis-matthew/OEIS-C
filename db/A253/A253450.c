/*
 * A253450  Number of (n+1) X (2+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) for n > 9. Initial
 *            values: a(1)=414, a(2)=1975, a(3)=4782, a(4)=8554, a(5)=15220,
 *            a(6)=31630, a(7)=74324, a(8)=188438, a(9)=502364
 * Range:     u8 none, u16 n <= 6, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Return the n-th term of sequence A253450.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* Static cache to allow arbitrary ordering of calls. */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;    /* number of terms currently stored */

    /* Fast path: value already computed. */
    if (n <= cache_size) return cache[n-1];

    /* Allocate larger array.  Copy old values, free old memory. */
    size_t old_size = cache_size;
    size_t new_size = (size_t)n;
    seq_t *new_cache = (seq_t*)calloc(new_size, sizeof(seq_t));
    if (!new_cache) return 0;  /* allocation failed (unlikely) */

    if (cache) {
        memcpy(new_cache, cache, old_size * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_size = new_size;

    /* Known initial values for indices 1..9. */
    static const seq_t init[9] = {
        (seq_t)414ULL, (seq_t)1975ULL, (seq_t)4782ULL, (seq_t)8554ULL,
        (seq_t)15220ULL, (seq_t)31630ULL, (seq_t)74324ULL,
        (seq_t)188438ULL, (seq_t)502364ULL
    };

    /* Compute missing terms up to n. */
    for (idx_t i = old_size + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i-1] = init[i-1];
        } else {
            /* Recurrence: a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3). */
            cache[i-1] = 6 * cache[i-2] - 11 * cache[i-3] + 6 * cache[i-4];
        }
    }

    return cache[n-1];
}
