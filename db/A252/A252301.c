/*
 * A252301  Number of (n+2)X(4+2) 0..3 arrays with every 3X3 subblock row and
 *          diagonal sum equal to 0 1 3 6 or 7 and every 3X3 column and
 *          antidiagonal sum not equal to 0 1 3 6 or 7.
 * https://oeis.org/A252301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) … a(18) as given, and for n > 18 a(n) = 7*a(n-3) -
 *            14*a(n-6) + 8*a(n-9) (OEIS A252301)
 * Range:     u8 none, u16 n <= 10, u32 n <= 34, u64 n <= 82, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t INIT[18] = {
    2670ULL, 1992ULL, 2287ULL, 3032ULL, 5117ULL,
    7489ULL, 10626ULL, 18971ULL, 28579ULL, 41037ULL,
    74183ULL, 112451ULL, 162093ULL, 294327ULL,
    447155ULL, 645357ULL, 1173623ULL, 1784435ULL
};

static seq_t *cache = NULL;      /* memoised terms */
static size_t cache_sz = 0;      /* number of initialized elements */
static size_t cache_cap = 0;     /* allocated capacity */

/* ensure cache has capacity for at least new_cap elements */
static void grow_cache(size_t new_cap) {
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* the problem guarantees sufficient memory, so no error handling needed */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for the sequence */
    size_t idx = (size_t)(n - 1);         /* zero‑based index */

    /* if value already computed, return it */
    if (idx < cache_sz) return cache[idx];

    /* allocate more space if needed */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap <= idx) new_cap <<= 1;
        grow_cache(new_cap);
    }

    /* compute all required terms up to idx */
    for (size_t i = cache_sz; i <= idx; ++i) {
        if (i < 18) {
            cache[i] = INIT[i];
        } else {
            /* recurrence using seq_t arithmetic */
            seq_t val = 7 * cache[i - 3];
            val += 8 * cache[i - 9];
            val -= 14 * cache[i - 6];
            cache[i] = val;
        }
    }
    cache_sz = idx + 1;
    return cache[idx];
}
