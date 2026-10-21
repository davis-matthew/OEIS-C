/*
 * A252315  Number of (n+2)X(2+2) 0..3 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 0 1 3 6 or 7.
 * https://oeis.org/A252315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)x4 arrays with entries 0..3 such that
 *            every 3x3 subblock row, column, diagonal and antidiagonal sum is
 *            NOT 0,1,3,6 or 7. The function constructs the adjacency matrix
 *            of valid transitions of triples of rows and performs dynamic
 *            programming
 * Range:     u8 none, u16 n <= 7, u32 n <= 18, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* global data */
static bool initialized = false;
static uint8_t row_vals[256][4];
static uint16_t *out_adj = NULL;        /* adjacency destinations */
static uint32_t *out_start = NULL;      /* start index per source */
static seq_t *prev_vec = NULL;
static seq_t *next_vec = NULL;
static uint16_t *active = NULL;
static uint16_t *next_active = NULL;
static size_t active_count = 0;

/* forbidden sums */
static bool forbidden[10] = {
    true,   /* 0 */
    true,   /* 1 */
    false,  /* 2 */
    true,   /* 3 */
    false,  /* 4 */
    false,  /* 5 */
    true,   /* 6 */
    true,   /* 7 */
    false,  /* 8 */
    false   /* 9 */
};

/* test whether a transition (prev2,prev1) -> cur is valid */
static inline bool valid_transition(uint16_t prev2, uint16_t prev1, uint16_t cur)
{
    /* cell values */
    uint8_t a0 = row_vals[prev2][0];
    uint8_t a1 = row_vals[prev2][1];
    uint8_t a2 = row_vals[prev2][2];
    uint8_t a3 = row_vals[prev2][3];
    uint8_t b0 = row_vals[prev1][0];
    uint8_t b1 = row_vals[prev1][1];
    uint8_t b2 = row_vals[prev1][2];
    uint8_t b3 = row_vals[prev1][3];
    uint8_t c0 = row_vals[cur][0];
    uint8_t c1 = row_vals[cur][1];
    uint8_t c2 = row_vals[cur][2];
    uint8_t c3 = row_vals[cur][3];

    /* subblock columns 0-2 */
    int s;
    s = a0 + a1 + a2; if (forbidden[s]) return false;
    s = b0 + b1 + b2; if (forbidden[s]) return false;
    s = c0 + c1 + c2; if (forbidden[s]) return false;
    s = a0 + b0 + c0; if (forbidden[s]) return false;
    s = a1 + b1 + c1; if (forbidden[s]) return false;
    s = a2 + b2 + c2; if (forbidden[s]) return false;
    s = a0 + b1 + c2; if (forbidden[s]) return false;
    s = a2 + b1 + c0; if (forbidden[s]) return false;

    /* subblock columns 1-3 */
    s = a1 + a2 + a3; if (forbidden[s]) return false;
    s = b1 + b2 + b3; if (forbidden[s]) return false;
    s = c1 + c2 + c3; if (forbidden[s]) return false;
    s = a1 + b1 + c1; if (forbidden[s]) return false;
    s = a2 + b2 + c2; if (forbidden[s]) return false;
    s = a3 + b3 + c3; if (forbidden[s]) return false;
    s = a1 + b2 + c3; if (forbidden[s]) return false;
    s = a3 + b2 + c1; if (forbidden[s]) return false;

    return true;
}

/* one‑time initialization of tables and adjacency */
static void init_tables(void)
{
    size_t i, j, k, src;
    size_t total_edges;
    uint32_t *deg = NULL;

    if (initialized) return;
    initialized = true;

    /* build row value table */
    for (i = 0; i < 256; ++i)
        for (j = 0; j < 4; ++j)
            row_vals[i][j] = (uint8_t)((i >> (j * 2)) & 3);

    /* first pass: count degrees */
    deg = (uint32_t*)calloc(65536, sizeof(uint32_t));
    for (i = 0; i < 256; ++i)
        for (j = 0; j < 256; ++j) {
            src = (i << 8) | j;
            for (k = 0; k < 256; ++k)
                if (valid_transition((uint16_t)i, (uint16_t)j, (uint16_t)k))
                    ++deg[src];
        }

    /* prefix sums */
    out_start = (uint32_t*)malloc((65537) * sizeof(uint32_t));
    out_start[0] = 0;
    for (i = 0; i < 65536; ++i)
        out_start[i + 1] = out_start[i] + deg[i];

    total_edges = out_start[65536];
    out_adj = (uint16_t*)malloc(total_edges * sizeof(uint16_t));

    /* second pass: fill adjacency */
    for (i = 0; i < 256; ++i)
        for (j = 0; j < 256; ++j) {
            src = (i << 8) | j;
            uint32_t pos = out_start[src];
            for (k = 0; k < 256; ++k)
                if (valid_transition((uint16_t)i, (uint16_t)j, (uint16_t)k)) {
                    uint16_t dest = (uint16_t)((j << 8) | k);
                    out_adj[pos++] = dest;
                }
        }

    free(deg);

    /* allocate vectors and active lists */
    prev_vec      = (seq_t*)malloc(65536 * sizeof(seq_t));
    next_vec      = (seq_t*)malloc(65536 * sizeof(seq_t));
    active        = (uint16_t*)malloc(65536 * sizeof(uint16_t));
    next_active   = (uint16_t*)malloc(65536 * sizeof(uint16_t));
}

/* main sequence function */
seq_t a(idx_t n)
{
    size_t i, src;
    size_t step;
    const size_t V = 65536;

    if (n == (idx_t)0) return (seq_t)0;

    init_tables();

    /* initial vector: all 1 */
    for (i = 0; i < V; ++i) {
        prev_vec[i] = (seq_t)1;
        active[i] = (uint16_t)i;
    }
    active_count = V;

    /* DP steps: n times (add one row each step) */
    for (step = 0; step < n; ++step) {
        /* clear next vector */
        for (i = 0; i < V; ++i) next_vec[i] = (seq_t)0;
        size_t next_cnt = 0;

        for (i = 0; i < active_count; ++i) {
            src = (size_t)active[i];
            seq_t val = prev_vec[src];
            if (val == (seq_t)0) continue;

            for (uint32_t p = out_start[src]; p < out_start[src + 1]; ++p) {
                uint16_t dest = out_adj[p];
                if (next_vec[dest] == (seq_t)0)
                    next_active[next_cnt++] = dest;
                next_vec[dest] += val;
            }
        }

        /* swap vectors and active lists */
        seq_t *tmp_vec = prev_vec;
        prev_vec = next_vec;
        next_vec = tmp_vec;

        uint16_t *tmp_act = active;
        active = next_active;
        next_active = tmp_act;
        active_count = next_cnt;
    }

    /* sum all counts */
    seq_t result = (seq_t)0;
    for (i = 0; i < V; ++i) result += prev_vec[i];
    return result;
}
