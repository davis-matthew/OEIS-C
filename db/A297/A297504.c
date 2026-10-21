/*
 * A297504  Number of nX6 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1 or 2 neighboring 1s.
 * https://oeis.org/A297504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula computing A297504:
   number of n×6 binary matrices where every 1 has 1 or 2
   neighbors among the horizontal and diagonal directions
   (no vertical adjacency). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define COLS 6
#define ROWS (1 << COLS)      /* 2^6 = 64 possible rows */
#define STATES (ROWS * ROWS)  /* state = (prev_row,curr_row) */

static uint8_t  good[ROWS][ROWS][ROWS];          /* good[prev][curr][next] */
static uint8_t  next_cnt[STATES];                /* number of allowed next rows */
static uint16_t next_map[STATES][ROWS];          /* list of allowed next rows */
static uint8_t  good_last[STATES];               /* state followed by 0 row */
static bool initialized = false;

/* Pre‑compute all transitions between row states. */
static void precompute(void)
{
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int curr = 0; curr < ROWS; ++curr) {
            int state = prev * ROWS + curr;
            uint8_t c = 0;
            good_last[state] = 0;
            for (int next = 0; next < ROWS; ++next) {
                bool ok = true;
                for (int col = 0; col < COLS && ok; ++col) {
                    if ((curr & (1 << col)) == 0) continue;  /* cell is 0 */
                    int nb = 0;
                    /* horizontal neighbours */
                    if (col > 0           && (curr & (1 << (col - 1)))) nb++;
                    if (col + 1 < COLS    && (curr & (1 << (col + 1)))) nb++;
                    /* diagonal neighbours from previous row */
                    if (col > 0           && (prev & (1 << (col - 1)))) nb++;
                    if (col + 1 < COLS    && (prev & (1 << (col + 1)))) nb++;
                    /* diagonal neighbours from next row */
                    if (col > 0           && (next & (1 << (col - 1)))) nb++;
                    if (col + 1 < COLS    && (next & (1 << (col + 1)))) nb++;
                    if (nb != 1 && nb != 2) { ok = false; break; }
                }
                good[prev][curr][next] = ok ? 1 : 0;
                if (ok) {
                    next_map[state][c++] = (uint16_t)next;
                    if (next == 0) good_last[state] = 1;
                }
            }
            next_cnt[state] = c;
        }
    }
    initialized = true;
}

/* Main sequence entry point: a(n) with offset 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1 */
    if (!initialized) precompute();

    static seq_t cur[STATES];
    static seq_t nxt[STATES];

    /* Initial state: previous row all zeros, current row arbitrary. */
    memset(cur, 0, sizeof(cur));
    for (int curr = 0; curr < ROWS; ++curr) {
        cur[ curr ] = (seq_t)1;      /* state (0,curr) */
    }

    /* Perform n-1 transitions. */
    for (idx_t step = 1; step < n; ++step) {
        memset(nxt, 0, sizeof(nxt));
        for (int s = 0; s < STATES; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            int cnt = next_cnt[s];
            int curr = s % ROWS;          /* current row of state (prev,curr) */
            uint16_t *list = next_map[s];
            for (int k = 0; k < cnt; ++k) {
                int next = list[k];
                int next_state = curr * ROWS + next;   /* new state (curr,next) */
                nxt[next_state] += val;
            }
        }
        memcpy(cur, nxt, sizeof(cur));
    }

    /* Sum counts of valid final states (next row zero). */
    seq_t result = 0;
    for (int s = 0; s < STATES; ++s) {
        if (good_last[s]) result += cur[s];
    }
    return result;
}
