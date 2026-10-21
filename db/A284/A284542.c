/*
 * A284542  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 974", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A284542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=7, a(3)=13, a(4)=29, a(5)=53, a(6)=113,
 *            a(7)=219, a(8)=479; For n>8: a(n)=a(n-1)+16*a(n-4)-16*a(n-5)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t last = 0;

static void init_cache(void) {
    if (cache != NULL) return;          /* already initialized */
    /* allocate space for the first 9 known values */
    cap = 9;
    cache = (seq_t*)calloc(cap, sizeof(seq_t));
    if (cache == NULL) exit(EXIT_FAILURE);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)7;
    cache[3] = (seq_t)13;
    cache[4] = (seq_t)29;
    cache[5] = (seq_t)53;
    cache[6] = (seq_t)113;
    cache[7] = (seq_t)219;
    cache[8] = (seq_t)479;
    last = 8;
}

static void ensure_capacity(size_t needed) {
    if (needed < cap) return;
    size_t newcap = (cap == 0 ? 16 : cap << 1);
    while (newcap <= needed) newcap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
    if (tmp == NULL) exit(EXIT_FAILURE);
    cache = tmp;
    cap = newcap;
}

seq_t a(idx_t n) {
    init_cache();
    if (n <= last) return cache[n];
    /* extend the cache up to n */
    ensure_capacity((size_t)n + 1);
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = cache[i-1] +
                   ((seq_t)16 * cache[i-4]) -
                   ((seq_t)16 * cache[i-5]);
    }
    last = n;
    return cache[n];
}
