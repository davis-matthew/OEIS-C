/*
 * A264491  Number of (2+1) X (n+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change 2,-1 1,0 2,1 0,-1 -2,-2
 *          or -1,0.
 * https://oeis.org/A264491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 63, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 3*a(n-2) + 2*a(n-3) + a(n-4) - a(n-5)  (n ≥ 6)
 * Initial values: a(1)=3, a(2)=1, a(3)=10, a(4)=8, a(5)=35
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* dynamic array of computed a(n) values */
static size_t cache_cap = 0;         /* current allocated capacity (≥ n+1) */
static idx_t computed_max = 0;       /* largest n whose value is computed */

/* Ensure the cache has space for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)(n + 1))
        new_cap <<= 1;          /* double until big enough */

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)          /* allocation failed */
        exit(EXIT_FAILURE);

    /* copy existing data */
    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    } else {
        memset(new_cache, 0, new_cap * sizeof(seq_t));
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)           /* the offset is 1; a(0) is undefined but return 0 */
        return 0;

    ensure_capacity(n);

    for (idx_t i = computed_max + 1; i <= n; ++i) {
        switch (i) {
            case 1: cache[i] = 3;          break;
            case 2: cache[i] = 1;          break;
            case 3: cache[i] = 10;         break;
            case 4: cache[i] = 8;          break;
            case 5: cache[i] = 35;         break;
            default:
                cache[i] = 3 * cache[i - 2]
                         + 2 * cache[i - 3]
                         +     cache[i - 4]
                         -     cache[i - 5];
                break;
        }
    }
    computed_max = n;
    return cache[n];
}
