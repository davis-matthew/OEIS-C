/*
 * A309154  Function of natural numbers satisfying the properties a(2*n) =
 *          2*a(n) and a(2*n+1) = -3 + 2*a(3*n+2).
 * https://oeis.org/A309154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(2*n) = 2*a(n) and a(2*n+1) = -3 + 2*a(3*n+2) (a(0) = 0, a(1) =
 *            1)
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define CACHE_SIZE (1U << 20)          /* 1 048 576 entries – plenty for n up to 10 000   */
#define EMPTY_KEY ((idx_t)-1)          /* sentinel value – all bits set */

/* Simple open‑addressing hash table for memoisation */
static idx_t *cache_keys  = NULL;      /* array of keys   */
static seq_t *cache_vals  = NULL;      /* array of values */
static size_t cache_cap   = CACHE_SIZE;

/* Forward declaration of the recursive routine */
static seq_t rec(idx_t n);

/* Initialise the cache once */
static void init_cache(void)
{
    if (cache_keys != NULL)
        return;                       /* already initialised */

    cache_keys = (idx_t*)calloc(cache_cap, sizeof(idx_t));
    cache_vals = (seq_t*)calloc(cache_cap, sizeof(seq_t));
    for (size_t i = 0; i < cache_cap; ++i)
        cache_keys[i] = EMPTY_KEY;
}

/* Lookup – returns 1 and stores the value in *out if found */
static int cache_lookup(idx_t key, seq_t *out)
{
    size_t h = key & (cache_cap - 1);   /* mask for power‑of‑two size */
    while (1) {
        if (cache_keys[h] == EMPTY_KEY)
            return 0;                   /* not found */
        if (cache_keys[h] == key) {
            *out = cache_vals[h];
            return 1;                   /* found */
        }
        h = (h + 1) & (cache_cap - 1);  /* linear probing */
    }
}

/* Insert into the hash table – replaces existing entry */
static void cache_insert(idx_t key, seq_t val)
{
    size_t h = key & (cache_cap - 1);
    while (1) {
        if (cache_keys[h] == EMPTY_KEY) {
            cache_keys[h] = key;
            cache_vals[h] = val;
            return;
        }
        if (cache_keys[h] == key) {
            cache_vals[h] = val;         /* update existing value */
            return;
        }
        h = (h + 1) & (cache_cap - 1);
    }
}

/* Recursive evaluation with memoisation */
static seq_t rec(idx_t n)
{
    seq_t cached;
    if (cache_lookup(n, &cached))          /* already computed? */
        return cached;

    seq_t result;
    if (n == 0) {
        result = 0;                        /* base value a(0) = 0 */
    } else if (n == 1) {
        result = 1;                        /* base value a(1) = 1 */
    } else if ((n & 1) == 0) {             /* even: a(2*m) = 2*a(m) */
        result = rec(n >> 1);
        result *= 2;                       /* 2*a(m) */
    } else {                               /* odd: a(2*m+1) = -3 + 2*a(3*m+2) */
        idx_t m = n >> 1;                  /* m = (n-1)/2                     */
        idx_t k = 3 * m + 2;               /* k = 3*m+2 = (3*n+1)/2            */
        seq_t t = rec(k);                  /* recursively compute a(k)        */
        result = t;
        result *= 2;                       /* 2*a(k) */
        result -= 3;                       /* 2*a(k) - 3                      */
    }

    cache_insert(n, result);               /* store for future calls         */
    return result;
}

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    init_cache();
    return rec(n);
}
