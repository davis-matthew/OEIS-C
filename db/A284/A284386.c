/*
 * A284386  Fixed point of the morphism 0->1, 1->1101.
 * https://oeis.org/A284386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   the fixed point of the morphism * 0 → 1 * 1 → 1101 * Sequence
 *            a(n) is the n‑th symbol (1‑based) of this infinite word
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static state shared by all calls of a() */
static seq_t *cache      = NULL; /* current word */
static size_t cache_len  = 0;    /* number of entries currently stored */
static size_t cache_cap  = 0;    /* capacity of cache */

/* ensure the word contains at least n symbols (n ≥ 1) */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;

    /* initialise on first call */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;    /* start from the fixed point symbol ‘1’ */
        cache_len = 1;
    }

    /* grow the word until it is long enough */
    size_t target = (size_t)n;
    while (cache_len < target) {
        size_t zeros = 0, ones = 0;
        for (size_t i = 0; i < cache_len; ++i) {
            if (cache[i] == (seq_t)0) ++zeros;
            else                      ++ones;
        }
        size_t new_len = zeros + 4 * ones;   /* 0→1 (len 1), 1→1101 (len 4) */

        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);

        size_t pos = 0;
        for (size_t i = 0; i < cache_len; ++i) {
            if (cache[i] == (seq_t)0) {      /* 0 → 1 */
                new_cache[pos++] = (seq_t)1;
            } else {                         /* 1 → 1101 */
                new_cache[pos++] = (seq_t)1;
                new_cache[pos++] = (seq_t)1;
                new_cache[pos++] = (seq_t)0;
                new_cache[pos++] = (seq_t)1;
            }
        }

        free(cache);
        cache      = new_cache;
        cache_len  = new_len;
        cache_cap  = new_len;
    }
}

/* external function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for offset < 1 – return 0 */
    ensure_cache(n);
    return cache[n - 1];
}
