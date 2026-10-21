/*
 * A268970  Number of nX7 0..2 arrays with some element plus some horizontally
 *          or antidiagonally adjacent neighbor totalling two not more than
 *          once.
 * https://oeis.org/A268970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Empirical recurrence: a(n) = 84 a(n-1) – 2466 a(n-2) + 31428 a(n-3) – 206469 a(n-4)
 * + 750384 a(n-5) – 1513404 a(n-6) + 1574640 a(n-7) – 656100 a(n-8) for n ≥ 9
 * Initial values: a(1)=768, a(2)=40176, a(3)=1770048, a(4)=72081792,
 * a(5)=2799406656, a(6)=105294777120, a(7)=3870355944960, a(8)=139818682627488
 */

static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem) {
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *p = realloc(*ptr, new_cap * elem);
    if (!p) abort();
    *ptr = p;
    *cap = new_cap;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* indices below the offset are not defined */

    static seq_t *cache = NULL;          /* memoised values */
    static size_t cache_cap = 0, cache_len = 0; /* cache_cap: allocated size, cache_len: highest allocated index + 1 */

    /* Initialise the base values once */
    if (cache_len == 0) {
        size_t init_size = 9;             /* indices 0 … 8 */
        ensure_capacity(init_size, &cache_cap, (void **)&cache, sizeof(seq_t));

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)768ULL;
        cache[2] = (seq_t)40176ULL;
        cache[3] = (seq_t)1770048ULL;
        cache[4] = (seq_t)72081792ULL;
        cache[5] = (seq_t)2799406656ULL;
        cache[6] = (seq_t)105294777120ULL;
        cache[7] = (seq_t)3870355944960ULL;
        cache[8] = (seq_t)139818682627488ULL;

        cache_len = init_size;           /* next free index */
    }

    /* Extend the cache if necessary */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        ensure_capacity(new_len, &cache_cap, (void **)&cache, sizeof(seq_t));

        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i <= 8) continue;        /* already initialised */

            /* Compute the linear combination using only unsigned arithmetic.
             * We first sum all positive contributions, then subtract the
             * negative contributions once; this guarantees no underflow. */
            seq_t pos = 84ULL * cache[i-1]
                      + 31428ULL * cache[i-3]
                      + 750384ULL * cache[i-5]
                      + 1574640ULL * cache[i-7];

            seq_t neg = 2466ULL * cache[i-2]
                      + 206469ULL * cache[i-4]
                      + 1513404ULL * cache[i-6]
                      + 656100ULL * cache[i-8];

            cache[i] = pos - neg;
        }

        cache_len = new_len;
    }

    return cache[(size_t)n];
}
