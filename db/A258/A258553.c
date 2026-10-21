/*
 * A258553  Number of (n+1) X (7+1) 0..1 arrays with every 2 X 2 subblock
 *          ne-sw antidiagonal difference nondecreasing horizontally and nw+se
 *          diagonal sum nondecreasing vertically.
 * https://oeis.org/A258553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - 4*a(n-5) + 6*a(n-6) -
 *            4*a(n-7) + a(n-8) for n>15 Initial values: a(1)=2008, a(2)=894,
 *            a(3)=2542, a(4)=4313, a(5)=8612, a(6)=13833, a(7)=21653,
 *            a(8)=31254, a(9)=43655, a(10)=57928, a(11)=75822, a(12)=96565,
 *            a(13)=121226, a(14)=148915, a(15)=181429
 * Range:     u8 none, u16 n <= 10, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


static seq_t *cache  = NULL;          /* cache[n] == a(n); cache[0] unused   */
static size_t cache_cap = 0;          /* allocated size of cache             */
static idx_t  max_computed = 0;       /* highest n for which cache[n] is set */

/* Ensure that cache has room for indices up to n (inclusive). */
static void ensure_capacity(idx_t n) {
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;
    size_t new_cap = needed < 16 ? 16 : needed * 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL)
        abort();                     /* out‑of‑memory */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise cache with the first 15 terms. */
static void init_cache(void) {
    const seq_t init[16] = {
        0,          /* index 0 unused */
        2008, 894, 2542, 4313, 8612,
        13833, 21653, 31254, 43655, 57928,
        75822, 96565, 121226, 148915, 181429
    };
    ensure_capacity(15);
    for (size_t i = 1; i <= 15; ++i)
        cache[i] = init[i];
    max_computed = 15;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                  /* undefined, but return 0 for safety */

    if (cache == NULL)
        init_cache();

    ensure_capacity(n);

    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val = 4 * cache[i - 1];
            val -= 6 * cache[i - 2];
            val += 4 * cache[i - 3];
            val -= 4 * cache[i - 5];
            val += 6 * cache[i - 6];
            val -= 4 * cache[i - 7];
            val += cache[i - 8];
            cache[i] = val;
        }
        max_computed = n;
    }
    return cache[n];
}
