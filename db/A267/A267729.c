/*
 * A267729  Number of n X 2 0..1 arrays with every repeated value in every row
 *          and column greater than or equal to the previous repeated value.
 * https://oeis.org/A267729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2) - 19*a(n-3) * + 24*a(n-4) + 24*a(n-5)
 *            - 19*a(n-6) * - 6*a(n-7) + 6*a(n-8) - a(n-9) (n ≥ 10) * Initial
 *            terms: * a(1)=4, a(2)=16, a(3)=64, a(4)=225, * a(5)=784,
 *            a(6)=2601, a(7)=8464, * a(8)=26896, a(9)=84100
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 40, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for memoisation.
 * cache[i] stores a(i+1), i.e. a(1) is stored at cache[0].   */
static seq_t *cache = NULL;
static size_t cache_len = 0; /* number of terms currently cached (≥0) */
static size_t cache_cap = 0; /* allocated capacity of the cache array */

/* Ensure that the cache can store at least n terms (1‑based).  */

static void ensure_cache(idx_t n)
{
    if (n < 1)
        return;
    size_t needed = (size_t)n;         /* we store a(1) at cache[0] */
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) {
        abort();  /* allocation failed – caller cannot recover safely */
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) for the OEIS sequence A267729.
 *  n  – index, 1‑based
 *  The function is memoised; any call order is allowed.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Make sure we have room for n terms. */
    ensure_cache(n);

    /* If the requested term is already cached, return it. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)(n - 1)];

    /* Initial values for n = 1 .. 9.  */
    const int init_val_arr[9] = {
        4, 16, 64, 225, 784, 2601, 8464, 26896, 84100
    };

    /* Compute terms from cache_len up to n (inclusive). */
    size_t i = cache_len; /* 0‑based index into cache */
    for (; i < (size_t)n; ++i) {
        if (i < 9) {
            cache[i] = (seq_t)init_val_arr[i];
        } else {
            /* Recurrence for a(i+1) using the 9‑term linear recurrence. */
            seq_t val =
                6 * cache[i - 1] - 6 * cache[i - 2] - 19 * cache[i - 3]
                + 24 * cache[i - 4] + 24 * cache[i - 5]
                - 19 * cache[i - 6] - 6 * cache[i - 7]
                + 6 * cache[i - 8] - cache[i - 9];
            cache[i] = val;
        }
    }

    cache_len = n;
    return cache[(size_t)(n - 1)];
}
