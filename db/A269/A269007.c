/*
 * A269007  Number of n X 4 binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A269007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 40*a(n-2) + 8*a(n-3) + 92*a(n-4) - 32*a(n-5)
 *            - 64*a(n-6) for n > 7, with initial values: a(1)=5, a(2)=36,
 *            a(3)=305, a(4)=2136, a(5)=14240, a(6)=91048, a(7)=566656
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Grow cache to at least n (1‑based).  Index 0 is unused. */
static void ensure_cap(idx_t n) {
    if ((size_t)n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < (size_t)n)
        new_cap = (size_t)n;
    cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    /* Zero initialise newly allocated part */
    for (size_t i = cache_cap + 1; i <= new_cap; ++i)
        cache[i] = 0;
    /* Set initial values if first time */
    if (cache_cap == 0) {
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)305;
        cache[4] = (seq_t)2136;
        cache[5] = (seq_t)14240;
        cache[6] = (seq_t)91048;
        cache[7] = (seq_t)566656;
    }
    cache_cap = new_cap;
}

/* Public function to obtain a(n) for n >= 1 */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;          /* sequence is undefined below 1; return 0 for safety */
    ensure_cap(n);
    if (n <= 7)
        return cache[n];
    for (idx_t i = 8; i <= n; ++i) {
        if (cache[i] == 0) {
            seq_t v = 12 * cache[i - 1]
                    - 40 * cache[i - 2]
                    + 8 * cache[i - 3]
                    + 92 * cache[i - 4]
                    - 32 * cache[i - 5]
                    - 64 * cache[i - 6];
            cache[i] = v;
        }
    }
    return cache[n];
}
