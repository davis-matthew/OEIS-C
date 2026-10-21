/*
 * A297517  Number of nX6 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1 or 3 neighboring 1s.
 * https://oeis.org/A297517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of nX6 0..1 arrays with every 1 horizontally, diagonally
 *            or antidiagonally adjacent to 1 or 3 neighboring 1s (Column 6 of
 *            A297519)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline bool valid_row(uint8_t prev, uint8_t curr, uint8_t nxt) {
    for (int c = 0; c < 6; ++c) {
        if (!((curr >> c) & 1))
            continue;
        int sum = 0;
        if (c > 0  && ((prev >> (c-1)) & 1)) sum++;
        if (c < 5  && ((prev >> (c+1)) & 1)) sum++;
        if (c > 0  && ((curr >> (c-1)) & 1)) sum++;
        if (c < 5  && ((curr >> (c+1)) & 1)) sum++;
        if (c > 0  && ((nxt  >> (c-1)) & 1)) sum++;
        if (c < 5  && ((nxt  >> (c+1)) & 1)) sum++;
        if (sum != 1 && sum != 3)
            return false;
    }
    return true;
}

/* Lazily build transition masks and final-check table */
static inline void build_tables(uint64_t trans[64][64], uint8_t final_ok[64][64]) {
    static bool done = false;
    if (done) return;
    for (int prev = 0; prev < 64; ++prev) {
        for (int curr = 0; curr < 64; ++curr) {
            uint64_t mask = 0;
            for (int nxt = 0; nxt < 64; ++nxt)
                if (valid_row(prev, curr, (uint8_t)nxt))
                    mask |= 1ULL << nxt;
            trans[prev][curr] = mask;
            final_ok[prev][curr] = valid_row(prev, curr, 0) ? 1 : 0;
        }
    }
    done = true;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;

    static uint64_t trans[64][64];
    static uint8_t  final_ok[64][64];
    build_tables(trans, final_ok);

    /* DP buffers */
    static seq_t dpA[64][64];
    static seq_t dpB[64][64];

    /* initialise dpA for first row */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            dpA[i][j] = 0;
    for (int curr = 0; curr < 64; ++curr)
        dpA[0][curr] = 1;      /* prev row is all zeros */

    /* process rows 1 .. n-1 */
    for (idx_t step = 1; step < n; ++step) {
        /* zero dpB */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dpB[i][j] = 0;
        for (int prev = 0; prev < 64; ++prev) {
            for (int curr = 0; curr < 64; ++curr) {
                seq_t val = dpA[prev][curr];
                if (!val) continue;
                uint64_t mask = trans[prev][curr];
                for (int nxt = 0; nxt < 64; ++nxt)
                    if (mask & (1ULL << nxt))
                        dpB[curr][nxt] += val;
            }
        }
        /* copy dpB to dpA */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dpA[i][j] = dpB[i][j];
    }

    /* final accumulation */
    seq_t result = 0;
    for (int prev = 0; prev < 64; ++prev) {
        for (int curr = 0; curr < 64; ++curr) {
            seq_t val = dpA[prev][curr];
            if (!val) continue;
            if (final_ok[prev][curr])
                result += val;
        }
    }
    return result;
}
