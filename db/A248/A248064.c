/*
 * A248064  Number of length n+5 0..4 arrays with some disjoint triples in
 *          each consecutive six terms having the same sum.
 * https://oeis.org/A248064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of arrays of length n+5 over {0,1,2,3,4} such that
 *            in each block of 6 consecutive terms there exists a partition
 *            into two triples with equal sums
 * Range:     u8 none, u16 n <= 3, u32 n <= 16, u64 n <= 41, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static const idx_t NUM_STATES = 3125;      /* 5^5 */
static const idx_t NUM_STATES6 = 15625;    /* 5^6 */
static const idx_t POW5_4 = 625;           /* 5^4 */

/* helper to precompute validity of every 6‑digit pattern */
static bool *valid_pattern(void)
{
    static bool *table = NULL;
    if (table != NULL)
        return table;
    table = (bool *)malloc(NUM_STATES6 * sizeof(bool));
    for (idx_t pat = 0; pat < NUM_STATES6; ++pat) {
        uint8_t d[6];
        idx_t tmp = pat;
        uint32_t sum = 0;
        for (int i = 0; i < 6; ++i) {
            d[i] = tmp % 5;
            sum += d[i];
            tmp /= 5;
        }
        if ((sum & 1) != 0) {
            table[pat] = false;
            continue;
        }
        uint32_t target = sum >> 1;
        bool ok = false;
        /* enumerate all 3‑subsets of indices {0..5} (20 combinations) */
        for (int i = 0; i < 6 && !ok; ++i)
            for (int j = i + 1; j < 6 && !ok; ++j)
                for (int k = j + 1; k < 6 && !ok; ++k) {
                    uint32_t s = d[i] + d[j] + d[k];
                    if (s == target) ok = true;
                }
        table[pat] = ok;
    }
    return table;
}

/* helper to precompute state shift (drop oldest digit, multiply by 5) */
static idx_t *shift_state(void)
{
    static idx_t *table = NULL;
    if (table != NULL)
        return table;
    table = (idx_t *)malloc(NUM_STATES * sizeof(idx_t));
    for (idx_t s = 0; s < NUM_STATES; ++s)
        table[s] = (s % POW5_4) * 5;
    return table;
}

seq_t a(idx_t n)
{
    /* precomputed tables */
    bool *valid = valid_pattern();
    idx_t *shift = shift_state();

    /* arrays for DP */
    seq_t *dp_prev = (seq_t *)calloc(NUM_STATES, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(NUM_STATES, sizeof(seq_t));

    /* initial state: length 5, all patterns allowed with count 1 */
    for (idx_t s = 0; s < NUM_STATES; ++s)
        dp_prev[s] = (seq_t)1;

    /* iterate adding one more element n times to reach length n+5 */
    for (idx_t step = 0; step < n; ++step) {
        /* zero next row */
        for (idx_t i = 0; i < NUM_STATES; ++i)
            dp_next[i] = (seq_t)0;
        for (idx_t s = 0; s < NUM_STATES; ++s) {
            seq_t cnt = dp_prev[s];
            if (cnt == (seq_t)0) continue;
            for (uint8_t x = 0; x < 5; ++x) {
                idx_t pat = s * 5 + x;   /* 6‑digit pattern */
                if (!valid[pat]) continue;
                idx_t new_state = shift[s] + x;
                dp_next[new_state] += cnt;
            }
        }
        /* swap dp_prev and dp_next */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;
    }

    /* sum all valid arrays of length n+5 */
    seq_t total = (seq_t)0;
    for (idx_t s = 0; s < NUM_STATES; ++s)
        total += dp_prev[s];

    /* free temporary tables only if not static (here they are static) */
    free(dp_prev);
    free(dp_next);

    return total;
}
