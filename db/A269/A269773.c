/*
 * A269773  Number of length-n 0..5 arrays with every repeated value unequal
 *          to the previous repeated value plus one mod 5+1.
 * https://oeis.org/A269773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 20*a(n-2) - 25*a(n-3); with a(1) = 6, a(2) =
 *            36, a(3) = 216 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Internal cache */
static seq_t *cache       = NULL;   /* points to array of seq_t, index 0 unused */
static idx_t  cache_size  = 0;      /* allocated size (≥ index of last element) */
static idx_t  computed_upto = 0;    /* highest n for which a(n) has been computed */

/* Ensure the cache can hold at least indices 0..n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)               /* already large enough */
        return;

    idx_t new_size = (cache_size == 0) ? (n + 1) : cache_size;
    if (new_size < 4)                       /* need room for first three terms */
        new_size = 4;
    while (new_size <= n)
        new_size <<= 1;                    /* power‑of‑two growth */

    seq_t *new_buf = NULL;
    if (cache == NULL) {                   /* first allocation */
        new_buf = (seq_t *)calloc(new_size, sizeof(seq_t));
    } else {
        new_buf = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_buf) {                     /* zero newly allocated part */
            size_t new_part = new_size - cache_size;
            memset((void *)(new_buf + cache_size), 0, new_part * sizeof(seq_t));
        }
    }
    if (!new_buf) abort();

    cache = new_buf;
    cache_size = new_size;

    /* Initialise the base values if this is the first run */
    if (computed_upto < 3) {
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)216;
        computed_upto = 3;
    }
}

/* Compute a(n) for all missing indices up to n using the recurrence. */
static void build_up_to(idx_t n)
{
    if (n <= computed_upto)
        return;

    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        cache[i] = (seq_t)10 * cache[i-1]
                 - (seq_t)20 * cache[i-2]
                 - (seq_t)25 * cache[i-3];
    }
    computed_upto = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* outside defined range, safe default */

    ensure_cache(n);
    build_up_to(n);
    return cache[n];
}
