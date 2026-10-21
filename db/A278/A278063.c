/*
 * A278063  Relative of Hofstadter Q-sequence: a(n) = n for 1 <= n <= 11; a(n)
 *          = a(n-a(n-1)) + a(n-a(n-2)) for n > 11.
 * https://oeis.org/A278063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n for 1 <= n <= 11; a(n) = a(n-a(n-1)) + a(n-a(n-2)) for
 *            n > 11
 * Range:     u8 n <= 199, u16 n <= 199, u32 n <= 199, u64 n <= 199,
 *            u128 n <= 199, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Internal memoisation cache */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static int cache_initialised = 0;

/* Grow the cache to accommodate indices up to n (inclusive) */
static void grow_cache(idx_t n)
{
    if (n <= cache_size) return;
    size_t new_size = (cache_size > 0) ? cache_size : 16;
    while (new_size < (size_t)n) new_size <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
    if (!new_cache) exit(1);                 /* out‑of‑memory */
    /* Zero the newly allocated part */
    if (new_size > cache_size) {
        memset(new_cache + cache_size + 1, 0,
               (new_size - cache_size) * sizeof(seq_t));
    }
    cache = new_cache;
    cache_size = new_size;
}

/* Ensure the initial 11 values are stored */
static void initialise_cache(void)
{
    if (!cache_initialised) {
        grow_cache(11);
        for (idx_t i = 1; i <= 11; ++i) cache[i] = (seq_t)i;
        cache_initialised = 1;
    }
}

/* Recursive evaluation with memoisation */
static seq_t compute_value(idx_t n)
{
    if (n <= 11) return (seq_t)n;

    grow_cache(n);
    if (cache[n] != 0) return cache[n];

    /* Compute a(n-1) and a(n-2) */
    seq_t val_nm1 = compute_value(n - 1);
    seq_t val_nm2 = compute_value(n - 2);

    /* Compute the argument indices */
    idx_t idx1 = (val_nm1 <= n) ? n - (idx_t)val_nm1 : 0;
    idx_t idx2 = (val_nm2 <= n) ? n - (idx_t)val_nm2 : 0;

    /* If any index falls below 1, the sequence is undefined – return 0 */
    if (idx1 < 1 || idx2 < 1) {
        cache[n] = 0;
        return 0;
    }

    seq_t res = compute_value(idx1) + compute_value(idx2);
    cache[n] = res;
    return res;
}

/* Public function */
seq_t a(idx_t n)
{
    if ((n & 1) == 0 && n == 0) /* guard against invalid n if needed */
        return 0;
    initialise_cache();
    return compute_value(n);
}
