/*
 * A334514  Eventual period of a single cell in rule 107 cellular automaton in
 *          a cyclic universe of width n.
 * https://oeis.org/A334514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-4) - a(n-8) for n>24. Initial terms (a(1)..a(24))
 *            are computed by simulating rule 107 cellular automaton on a
 *            cyclic universe of width n
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


static const uint8_t rule_map[8] = {1, 1, 0, 1, 0, 1, 1, 0};

/*------------------------------------------------------------------
 * next_state:
 *   Compute the next state of a cyclic binary array of width w,
 *   given the current state as a uint64_t bit mask.
 *------------------------------------------------------------------*/
static uint64_t next_state(uint64_t s, uint64_t mask, int w)
{
    /* rotate left and right neighbors */
    uint64_t left  = (s >> 1) | ((s & 1) << (w - 1));
    uint64_t right = (s << 1) | ((s >> (w - 1)) & 1);

    uint64_t res = 0;
    for (int i = 0; i < w; ++i) {
        uint8_t pattern = ((left >> i) & 1) << 2 |
                          ((s >> i) & 1) << 1 |
                          ((right >> i) & 1);
        if (rule_map[pattern]) {
            res |= ((uint64_t)1 << i);
        }
    }
    return res & mask;
}

/*------------------------------------------------------------------
 * period_for_width:
 *   Return the eventual period of a single active cell in rule 107
 *   for a cyclic universe of width w (1 <= w <= 24).  Uses Floyd's
 *   cycle‑finding algorithm so no extra storage is required.
 *------------------------------------------------------------------*/
static seq_t period_for_width(idx_t w)
{
    uint64_t mask = (w == 64) ? ~0ULL : ((uint64_t)1 << w) - 1;
    uint64_t start = 1ULL;                 /* one cell active */

    /* Floyd's algorithm */
    uint64_t tortoise = next_state(start, mask, (int)w);
    uint64_t hare     = next_state(tortoise, mask, (int)w);
    while (tortoise != hare) {
        tortoise = next_state(tortoise, mask, (int)w);
        hare     = next_state(next_state(hare, mask, (int)w), mask,
                              (int)w);
    }

    /* find the start of the cycle (mu) */
    tortoise = start;
    while (tortoise != hare) {
        tortoise = next_state(tortoise, mask, (int)w);
        hare     = next_state(hare, mask, (int)w);
    }

    /* find the cycle length (lambda) */
    uint64_t lambda = 1;
    hare = next_state(tortoise, mask, (int)w);
    while (tortoise != hare) {
        hare = next_state(hare, mask, (int)w);
        ++lambda;
    }

    return (seq_t)lambda;
}

/*------------------------------------------------------------------
 * a:
 *   Memoised computation of the OEIS sequence A334514.
 *------------------------------------------------------------------*/
static seq_t *cache   = NULL;   /* cache[0] holds a(1), cache[1] holds a(2), ... */
static idx_t cache_sz = 0;      /* number of cached terms */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, arbitrary return */

    /* grow cache if needed */
    if (cache_sz < n) {
        idx_t new_len = n;
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            new_len * sizeof(seq_t));
        /* realloc(NULL, ...) behaves the same as malloc() */
        if (!new_cache) abort();
        cache = new_cache;

        for (idx_t i = cache_sz; i < n; ++i) {
            if (i < 24) {
                cache[i] = period_for_width((idx_t)(i + 1));
            } else {
                /* a(n) = 2*a(n-4) - a(n-8) for n > 24 */
                cache[i] = (cache[i - 4] << 1) - cache[i - 8];
            }
        }
        cache_sz = n;
    }

    return cache[n - 1];
}
