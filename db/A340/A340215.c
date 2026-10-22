/*
 * A340215  Consider constructing binary words that begin with 0 such that the
 *          subword 00, whenever it appears, is followed by 111. Then a(n)
 *          counts such words at length n (including those where the string
 *          111 is yet being completed - see Example).
 * https://oeis.org/A340215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-5) with a(1)=1, a(2)=2, a(3)=3,
 *            a(4)=5, a(5)=8
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 42, u64 n <= 84, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc, abort */
#include <stddef.h>   /* size_t */

static seq_t *cache = NULL;   /* indexed by n, cache[0] unused */
static idx_t cache_size = 0;  /* highest n that has been filled in cache */

/* enlarge the cache so that it can store values up to index n */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n)
        return;

    /* Guarantee at least indices 0..5, and up to n */
    idx_t new_size = (n + 1 > 6) ? (n + 1) : 6;

    seq_t *tmp = NULL;
#ifdef __cplusplus
    tmp = reinterpret_cast<seq_t *>(realloc(cache, (size_t)new_size * sizeof(seq_t)));
#else
    tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
#endif
    if (!tmp)
        abort();  /* out of memory */
    cache = tmp;

    /* Initialise new slots */
    for (idx_t i = cache_size + 1; i < new_size; ++i) {
        if (i <= 5) {
            /* base values */
            switch (i) {
                case 1: cache[1] = (seq_t)1; break;
                case 2: cache[2] = (seq_t)2; break;
                case 3: cache[3] = (seq_t)3; break;
                case 4: cache[4] = (seq_t)5; break;
                case 5: cache[5] = (seq_t)8; break;
            }
        } else {
            /* recurrence: a(n) = a(n-1)+a(n-2)+a(n-5) */
            cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 5];
        }
    }

    cache_size = new_size - 1;   /* last valid index is new_size-1 */
}

seq_t a(idx_t n)
{
    if (n < 1)          /* sequence is undefined for n<1 */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
