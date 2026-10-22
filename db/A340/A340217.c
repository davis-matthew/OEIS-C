/*
 * A340217  Consider binary words that begin with 1 such that the subword 00,
 *          whenever it appears, is followed by 111. Then a(n) counts such
 *          words at length n (including those where the string 111 is yet
 *          being completed - see Example).
 * https://oeis.org/A340217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-5) with
 *            a(1)=1,a(2)=2,a(3)=4,a(4)=7,a(5)=12
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 41, u64 n <= 83, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Memoised recurrence implementation */
static seq_t *cache = NULL;          /* array of computed terms (index 0 -> a(1))   */
static size_t cache_capacity = 0;    /* allocated array size                       */
static size_t cache_computed  = 0;    /* number of terms actually filled (>=5)      */

static void init_cache(void) {
    /* initialise with the first 5 terms; allocate capacity 6 */
    cache_capacity = 6;
    cache = (seq_t*)malloc(cache_capacity * sizeof(seq_t));
    if (!cache) exit(1);
    cache[0] = 1;   /* a(1) */
    cache[1] = 2;   /* a(2) */
    cache[2] = 4;   /* a(3) */
    cache[3] = 7;   /* a(4) */
    cache[4] = 12;  /* a(5) */
    /* initialise unused element to zero (not strictly needed) */
    cache[5] = 0;
    cache_computed = 5;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* not defined but safe fallback */

    if (cache == NULL) init_cache();

    /* compute additional terms if required */
    if ((size_t)n > cache_computed) {
        /* enlarge buffer if needed */
        size_t needed = (size_t)n;
        if (needed > cache_capacity) {
            size_t new_cap = cache_capacity;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            /* initialise new slots to zero (not strictly necessary) */
            for (size_t i = cache_capacity; i < new_cap; ++i) cache[i] = 0;
            cache_capacity = new_cap;
        }

        /* fill missing terms using the recurrence */
        for (size_t i = cache_computed; i < (size_t)n; ++i) {
            /* i corresponds to a(i+1) */
            cache[i] = cache[i-1] + cache[i-2] + cache[i-5];
        }
        cache_computed = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
