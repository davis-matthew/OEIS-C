/*
 * A282590  Number of n X 5 0..1 arrays with no 1 equal to more than two of
 *          its king-move neighbors, with the exception of exactly two
 *          elements.
 * https://oeis.org/A282590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n × 5 binary arrays with exactly two cells that
 *            have more than two king‑move neighbors (all other cells have at
 *            most two)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define WIDTH     5
#define MASKS    (1 << WIDTH)          /* 32 possible row masks */
#define DP_STATES  (MASKS * MASKS * 3) /* prev, curr, violations (0..2) */

/* 3‑row delta table: delta_table[p][c][n] = #violations in row c
   when the rows above = p, current = c, below = n. */
static uint8_t delta_table[MASKS][MASKS][MASKS];
static bool delta_inited = false;

static void init_delta_table(void)
{
    if (delta_inited) return;
    for (int p = 0; p < MASKS; ++p) {
        for (int c = 0; c < MASKS; ++c) {
            for (int n = 0; n < MASKS; ++n) {
                unsigned viol = 0;
                for (int j = 0; j < WIDTH; ++j) {
                    if (!(c & (1 << j))) continue; /* only for 1‑cells */

                    int neigh = 0;
                    /* neighbors in previous row */
                    if (j > 0   && (p & (1 << (j - 1)))) ++neigh;
                    if (p & (1 << j)) ++neigh;
                    if (j < WIDTH - 1 && (p & (1 << (j + 1)))) ++neigh;
                    /* neighbors in current row */
                    if (j > 0   && (c & (1 << (j - 1)))) ++neigh;
                    if (j < WIDTH - 1 && (c & (1 << (j + 1)))) ++neigh;
                    /* neighbors in next row */
                    if (j > 0   && (n & (1 << (j - 1)))) ++neigh;
                    if (n & (1 << j)) ++neigh;
                    if (j < WIDTH - 1 && (n & (1 << (j + 1)))) ++neigh;

                    if (neigh > 2) ++viol;
                }
                delta_table[p][c][n] = (uint8_t)viol;
            }
        }
    }
    delta_inited = true;
}

static void zero_array(seq_t *arr, size_t len)
{
    for (size_t i = 0; i < len; ++i) arr[i] = (seq_t)0;
}

/* DP tables */
static seq_t dp[DP_STATES];
static seq_t dp_next[DP_STATES];

#define IDX(prev,curr,v) ((((prev) * MASKS) + (curr)) * 3 + (v))

seq_t a(idx_t n)
{
    init_delta_table();

    /* pointers to the current and next DP tables */
    seq_t *cur = dp;
    seq_t *next = dp_next;

    zero_array(cur, DP_STATES);
    cur[IDX(0,0,0)] = (seq_t)1;   /* start with two empty rows */

    for (idx_t step = 1; step <= n + 1; ++step) {
        bool any_next = (step <= n);   /* during real rows consider all rows */
        zero_array(next, DP_STATES);

        for (int prev = 0; prev < MASKS; ++prev) {
            for (int curr = 0; curr < MASKS; ++curr) {
                for (int v = 0; v <= 2; ++v) {
                    seq_t val = cur[IDX(prev,curr,v)];
                    if (val == (seq_t)0) continue;

                    if (any_next) {
                        for (int nxt = 0; nxt < MASKS; ++nxt) {
                            uint8_t d = delta_table[prev][curr][nxt];
                            int nv = v + d;
                            if (nv > 2) continue;
                            next[IDX(curr,nxt,nv)] += val;
                        }
                    } else { /* final step: forced next row is zero */
                        int nv = v + delta_table[prev][curr][0];
                        if (nv <= 2) next[IDX(curr,0,nv)] += val;
                    }
                }
            }
        }
        /* swap current and next tables for the next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum states where the last row is zero and exactly two violations occurred */
    seq_t result = (seq_t)0;
    for (int prev = 0; prev < MASKS; ++prev) {
        result += cur[IDX(prev,0,2)];
    }
    return result;
}
