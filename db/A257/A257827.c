/*
 * A257827  Positive integers whose square is the sum of 96 consecutive
 *          squares.
 * https://oeis.org/A257827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-8) - a(n-16) for n>=17, with initial values
 *            a(1)=652, a(2)=724, a(3)=788, a(4)=1012, a(5)=1828, a(6)=2372,
 *            a(7)=2596, a(8)=2908, a(9)=6164, a(10)=6908, a(11)=7564,
 *            a(12)=9836, a(13)=17996, a(14)=23404, a(15)=25628, a(16)=28724
 * Range:     u8 none, u16 n <= 17, u32 n <= 56, u64 n <= 133, u128 n <= 288,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_terms[16] = {
    (seq_t)652, (seq_t)724, (seq_t)788, (seq_t)1012,
    (seq_t)1828, (seq_t)2372, (seq_t)2596, (seq_t)2908,
    (seq_t)6164, (seq_t)6908, (seq_t)7564, (seq_t)9836,
    (seq_t)17996, (seq_t)23404, (seq_t)25628, (seq_t)28724
};

static seq_t *cache = NULL;        /* 1‑indexed cache, cache[0] unused */
static size_t cache_size = 0;      /* number of allocated entries (>=0) */

seq_t a(idx_t n)
{
    /* Undefined for n < 1 */
    if (n < 1) return (seq_t)0;

    /* Convert index to a size_t for array use */
    size_t N = (size_t)n;

    /* Grow cache if necessary */
    if (N >= cache_size) {
        size_t new_size = N + 1;   /* include index 0 */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }

        /* Initialize the dummy element at index 0 */
        new_cache[0] = (seq_t)0;

        /* Fill new entries */
        size_t i, start;
        if (cache_size == 0) {
            start = 1;               /* first new index is 1 */
        } else {
            start = cache_size;      /* first new index after existing data */
        }

        const seq_t TEN = (seq_t)10;
        for (i = start; i <= N; ++i) {
            if (i <= 16) {
                new_cache[i] = base_terms[i - 1];
            } else {
                new_cache[i] = TEN * new_cache[i - 8] - new_cache[i - 16];
            }
        }

        /* Commit the enlarged cache */
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[N];
}
