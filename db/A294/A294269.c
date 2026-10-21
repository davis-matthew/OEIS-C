/*
 * A294269  a(n) is the smallest number not already in the sequence which
 *          shares a factor with an even number of preceding terms; a(1) = 1.
 * https://oeis.org/A294269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=6, a(6)=4, a(7)=7; For n≥8:
 *            a(n)=a(n-1)+a(n-4)-a(n-5) (Recurrence given in the OEIS entry)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

/* External link: seq_t and idx_t are provided by the build. */

static const seq_t INITIAL[8] = {0, 1, 2, 3, 5, 6, 4, 7};

seq_t a(idx_t n)
{
    /* Handle the first seven terms directly. */
    if (n <= 7) {
        return INITIAL[n];
    }

    /* Static cache to store computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of allocated slots (>= cache_size) */
    static idx_t last = 7;         /* index up to which terms are computed */

    /* Allocate or resize the cache if necessary. */
    if (cache_size == 0) {
        cache_size = 64;                       /* start with a reasonable size */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        assert(cache != NULL);
        /* Initialize the first seven terms. */
        for (idx_t i = 1; i <= 7; ++i) {
            cache[i] = INITIAL[i];
        }
        /* Zero the rest to indicate “not yet computed”. */
        for (idx_t i = 8; i < cache_size; ++i) {
            cache[i] = 0;
        }
    }

    if (n >= cache_size) {
        /* Grow the cache to at least n+1 elements. */
        idx_t new_size = cache_size;
        while (new_size <= n) {
            new_size <<= 1;   /* double the size */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        assert(tmp != NULL);
        /* Zero the new slots. */
        for (idx_t i = cache_size; i < new_size; ++i) {
            tmp[i] = 0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute new terms if required. */
    for (idx_t k = last + 1; k <= n; ++k) {
        /* Recurrence: a(k) = a(k-1) + a(k-4) - a(k-5) */
        seq_t val = cache[k - 1] + cache[k - 4] - cache[k - 5];
        cache[k] = val;
    }
    last = n;

    return cache[n];
}
