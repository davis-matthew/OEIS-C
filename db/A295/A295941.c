/*
 * A295941  Number of nX6 0..1 arrays with each 1 adjacent to 1 or 3 king-move
 *          neighboring 1s.
 * https://oeis.org/A295941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 0/1 arrays with each 1 adjacent to 1 or 3
 *            king‑move neighbouring 1s. The code uses a transfer matrix DP
 *            over rows
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 204 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* test whether a 3‑row window (prev,curr,next) satisfies the adjacency
   rule for all 1‑cells of the middle row curr */
static bool is_valid_triplet(unsigned prev, unsigned curr, unsigned next) {
    for (int j = 0; j < 6; ++j) {
        if (!(curr & (1u << j)))
            continue;
        int cnt = 0;
        /* same row neighbours */
        if (j > 0 && (curr & (1u << (j - 1)))) ++cnt;
        if (j < 5 && (curr & (1u << (j + 1)))) ++cnt;
        /* row above neighbours */
        if (prev & (1u << j))                 ++cnt;
        if (j > 0 && (prev & (1u << (j - 1))))++cnt;
        if (j < 5 && (prev & (1u << (j + 1))))++cnt;
        /* row below neighbours */
        if (next & (1u << j))                 ++cnt;
        if (j > 0 && (next & (1u << (j - 1))))++cnt;
        if (j < 5 && (next & (1u << (j + 1))))++cnt;
        if (cnt != 1 && cnt != 3)
            return false;
    }
    return true;
}

#define STATES 64u
#define FULL (STATES * STATES)

/* --------------------------------------------------------------- */
/*  Main sequence function                                         */
/* --------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* static tables – built only once */
    static bool   built = false;
    static uint8_t  trans_count[FULL];
    static unsigned char trans_next[FULL][STATES];
    static unsigned char last_valid[STATES][STATES];
    static unsigned char first_valid[STATES];
    static seq_t dp[STATES][STATES];
    static seq_t ndp[STATES][STATES];

    if (!built) {
        /* transition table: for each pair (prev,curr) store all next rows that
           keep the middle row legal */
        for (unsigned id = 0; id < FULL; ++id)
            trans_count[id] = 0;

        for (unsigned prev = 0; prev < STATES; ++prev) {
            for (unsigned curr = 0; curr < STATES; ++curr) {
                unsigned id = prev * STATES + curr;
                for (unsigned next = 0; next < STATES; ++next) {
                    if (is_valid_triplet(prev, curr, next)) {
                        trans_next[id][trans_count[id]] = (unsigned char)next;
                        ++trans_count[id];
                    }
                }
            }
        }

        /* first and last row validity (sentinels 0 above/below) */
        for (unsigned m = 0; m < STATES; ++m)
            first_valid[m] = is_valid_triplet(0, m, 0);

        for (unsigned prev = 0; prev < STATES; ++prev)
            for (unsigned curr = 0; curr < STATES; ++curr)
                last_valid[prev][curr] = is_valid_triplet(prev, curr, 0);

        built = true;
    }

    /* ----------------------------------------------------------- */
    /*  Handle small n directly                                  */
    /* ----------------------------------------------------------- */
    if (n == 0) return (seq_t)0;   /* undefined for this sequence */
    if (n == 1) {
        seq_t ans = 0;
        for (unsigned m = 0; m < STATES; ++m)
            if (first_valid[m]) ++ans;
        return ans;
    }
    if (n == 2) {
        seq_t ans = 0;
        for (unsigned r1 = 0; r1 < STATES; ++r1)
            for (unsigned r2 = 0; r2 < STATES; ++r2)
                if (is_valid_triplet(0, r1, r2) && is_valid_triplet(r1, r2, 0))
                    ++ans;
        return ans;
    }

    /* ----------------------------------------------------------- */
    /*  Initialise DP with the first two rows                     */
    /* ----------------------------------------------------------- */
    for (unsigned i = 0; i < STATES; ++i)
        for (unsigned j = 0; j < STATES; ++j)
            dp[i][j] = 0;

    for (unsigned r1 = 0; r1 < STATES; ++r1)
        for (unsigned r2 = 0; r2 < STATES; ++r2)
            if (is_valid_triplet(0, r1, r2))
                dp[r1][r2] = (seq_t)1;

    /* ----------------------------------------------------------- */
    /*  Process rows 3 … n                                       */
    /* ----------------------------------------------------------- */
    for (idx_t step = 3; step <= n; ++step) {
        /* clear ndp */
        for (unsigned i = 0; i < STATES; ++i)
            for (unsigned j = 0; j < STATES; ++j)
                ndp[i][j] = 0;

        for (unsigned prev = 0; prev < STATES; ++prev) {
            for (unsigned curr = 0; curr < STATES; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;
                unsigned id = prev * STATES + curr;
                uint8_t tcnt = trans_count[id];
                for (uint8_t k = 0; k < tcnt; ++k) {
                    unsigned next = trans_next[id][k];
                    ndp[curr][next] += ways;
                }
            }
        }

        /* copy ndp back */
        for (unsigned i = 0; i < STATES; ++i)
            for (unsigned j = 0; j < STATES; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* ----------------------------------------------------------- */
    /*  Final row must be legal with sentinel 0 below             */
    /* ----------------------------------------------------------- */
    seq_t answer = 0;
    for (unsigned prev = 0; prev < STATES; ++prev)
        for (unsigned curr = 0; curr < STATES; ++curr)
            if (last_valid[prev][curr])
                answer += dp[prev][curr];

    return answer;
}
