/*
 * A317763  Number of nX7 0..1 arrays with every element unequal to 0, 1 or 2
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A317763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-4) + a(n-5) - a(n-6) + a(n-7)
 *            for n > 11 Initial values: a(1)=64, a(2)=190, a(3)=196,
 *            a(4)=232, a(5)=302, a(6)=425, a(7)=639, a(8)=1012, a(9)=1663,
 *            a(10)=2801, a(11)=4792
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 35, u64 n <= 74, u128 n <= 153,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_n = 0;

/* initialise cache with the first 11 terms */
static void init_cache(void)
{
    /* allocate indices 0..11 (index 0 unused) */
    cache_cap = 12;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(1);
    cache[1] = 64;
    cache[2] = 190;
    cache[3] = 196;
    cache[4] = 232;
    cache[5] = 302;
    cache[6] = 425;
    cache[7] = 639;
    cache[8] = 1012;
    cache[9] = 1663;
    cache[10] = 2801;
    cache[11] = 4792;
    last_n = 11;
}

/* make sure cache can hold index n */
static void ensure_capacity(idx_t n)
{
    if (!cache) {
        init_cache();
    }
    if ((size_t)(n + 1) > cache_cap) {
        size_t newcap = cache_cap;
        while ((size_t)(n + 1) > newcap) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = newcap;
    }
}

/* external function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, return 0 safely */
    ensure_capacity(n);
    if (n <= last_n) return cache[n];

    for (idx_t i = last_n + 1; i <= n; ++i) {
        if (i > 11) {
            seq_t v = 3 * cache[(size_t)i - 1]
                      - 2 * cache[(size_t)i - 2]
                      -       cache[(size_t)i - 4]
                      +       cache[(size_t)i - 5]
                      -       cache[(size_t)i - 6]
                      +       cache[(size_t)i - 7];
            cache[(size_t)i] = v;
        }
        /* values for i <= 11 are already initialized */
    }
    last_n = n;
    return cache[n];
}
