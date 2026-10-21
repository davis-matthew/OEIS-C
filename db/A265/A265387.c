/*
 * A265387  Sequence defined by a(1)=a(2)=1 and a(n) = gray(a(n-1)) +
 *          gray(a(n-2)), with gray(m) = A003188(m).
 * https://oeis.org/A265387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A265387: a(1)=a(2)=1, a(n)=gray(a(n-1))+gray(a(n-2))
   where gray(m)=m^(m>>1) (binary-reflected Gray code). */

#include <stdlib.h>
#include <string.h>

/* static memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed_upto = 0;

/* ensure cache has room for indices 0..n (inclusive) */
static void ensure_capacity(idx_t n)
{
    /* n is 0‑based maximum index needed (so size n+1) */
    if ((idx_t)cache_cap < n + 1) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();                 /* out of memory */
        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            /* initialise newly allocated part to 0 */
            if (new_cap > cache_cap) {
                memset(new_cache + cache_cap, 0,
                       (new_cap - cache_cap) * sizeof(seq_t));
            }
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, but safe */
    if (n <= 2) return 1;                     /* base cases */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* need space for indices 0..2 */
        ensure_capacity(2);
        cache[1] = 1;
        cache[2] = 1;
        computed_upto = 2;
    }

    if (n <= computed_upto) return cache[n];

    /* extend cache if necessary */
    ensure_capacity(n);

    /* compute new terms */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t gray1 = prev1 ^ (prev1 >> 1);
        seq_t gray2 = prev2 ^ (prev2 >> 1);
        cache[i] = gray1 + gray2;
    }
    computed_upto = n;
    return cache[n];
}
