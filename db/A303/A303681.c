/*
 * A303681  Number of nX7 0..1 arrays with every element unequal to 0, 1 or 3
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 53, u64 n <= 115, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = a(n-1)+a(n-4)+a(n-8), for n > 14.
   Initial values:
   a(1)=21, a(2)=103, a(3)=57, a(4)=90, a(5)=150, a(6)=245,
   a(7)=367, a(8)=509, a(9)=697, a(10)=957, a(11)=1311,
   a(12)=1851, a(13)=2671, a(14)=3869. */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* 1‑based cache array */
static size_t cache_cap = 0;         /* allocated size (minus 1) */
static size_t computed = 0;          /* highest index for which a value is known */

/* Initial values for n = 1..14 */
static const seq_t base_vals[15] = {
    0ULL,   /* unused index 0 */
    21ULL, 103ULL, 57ULL, 90ULL, 150ULL, 245ULL, 367ULL,
    509ULL, 697ULL, 957ULL, 1311ULL, 1851ULL, 2671ULL, 3869ULL
};

/* Allocate or grow the cache array to at least 'need' (1‑based). */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) abort();            /* out of memory */
    cache = tmp;
    /* Initialise new entries to zero (supports integral and integer‑like types) */
    for (size_t i = cache_cap + 1; i <= new_cap; ++i) cache[i] = 0;
    cache_cap = new_cap;
}

/* Initialise base values if not already done. */
static void init_base(void)
{
    if (computed) return;         /* already initialised */
    ensure_cache(14);
    for (size_t i = 1; i <= 14; ++i) cache[i] = base_vals[i];
    computed = 14;
}

/* Main function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined but harmless */
    init_base();

    size_t idx = (size_t)n;       /* n is unsigned large; cast for indexing */

    if (idx <= computed) return cache[idx];

    /* Ensure capacity up to idx at least. */
    ensure_cache(idx);

    /* Compute missing values up to idx. */
    for (size_t i = computed + 1; i <= idx; ++i) {
        cache[i] = cache[i-1] + cache[i-4] + cache[i-8];
    }
    computed = idx;
    return cache[idx];
}
