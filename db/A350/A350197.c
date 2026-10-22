/*
 * A350197  a(n) is the smallest positive integer which can be represented as
 *          the sum of distinct nonzero hexagonal numbers in exactly n ways,
 *          or 0 if no such integer exists.
 * https://oeis.org/A350197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer that can be represented as a
 *            sum of distinct nonzero hexagonal numbers in exactly n ways; 0
 *            if none
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const idx_t MAX_N = 10000;
static seq_t *best = NULL;
static bool initialized = false;

static void init(void) {
    /* Use the first 35 hexagonal numbers.  Their sum is ~30k, enough to
       achieve at least 10000 distinct representation counts. */
    const size_t KMAX = 35;
    idx_t h_array[KMAX];
    idx_t sum_all = 0;
    for (size_t k = 1; k <= KMAX; ++k) {
        idx_t h = k * (2 * k - 1);
        h_array[k - 1] = h;
        sum_all += h;
    }
    size_t max_sum = (size_t)sum_all;

    uint16_t *cnt = (uint16_t *)calloc(max_sum + 1, sizeof(uint16_t));
    if (!cnt) return;                    /* unlikely, but guard against OOM */
    cnt[0] = 1;

    for (size_t i = 0; i < KMAX; ++i) {
        size_t h = (size_t)h_array[i];
        for (size_t s = max_sum; s >= h; --s) {
            uint16_t prev = cnt[s];
            if (prev < (MAX_N + 1)) {
                uint32_t newv = (uint32_t)prev + cnt[s - h];
                if (newv > (MAX_N + 1))
                    newv = (MAX_N + 1);
                cnt[s] = (uint16_t)newv;
            }
        }
    }

    best = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));
    if (!best) { free(cnt); return; }

    for (size_t s = 1; s <= max_sum; ++s) {
        uint16_t c = cnt[s];
        if (c <= MAX_N && best[c] == 0)
            best[c] = (seq_t)s;
    }

    free(cnt);
    initialized = true;
}

seq_t a(idx_t n) {
    if (n == 0 || n > MAX_N)
        return 0;
    if (!initialized) init();
    if (!initialized) return 0;            /* init failed or OOM */
    return best[n];
}
