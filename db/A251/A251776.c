/*
 * A251776  Number of (n+2)X(5+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum 3 or 6 and every diagonal and antidiagonal sum not 3 or
 *          6.
 * https://oeis.org/A251776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 18, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A251776
 * Computes the number of (n+2)×(5+2) arrays of digits 0..3
 * satisfying the stated subblock constraints.
 *
 * Method: Enumerate all admissible 7‑column row patterns
 * (every 3 consecutive columns sum to 3 or 6).  For each pair
 * of consecutive rows keep a state (previous two rows).  For
 * each state precompute all admissible next rows that respect
 * the column and diagonal conditions.  Then perform
 * n transitions of a dynamic program over these states,
 * starting from all pairs of rows.  The total of all final
 * states is a(n).  The algorithm is O(n * S) where
 * S = (#valid rows)^2 ≈ 1.3 × 10^8 in the worst case.
 *
 * The code is designed to compile under C11 or C++17.
 * No external libraries beyond the standard headers are used.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ********** Helper to allocate arrays of seq_t (works in C and C++) ********** */
static seq_t *seq_alloc(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    seq_t *p = (seq_t *)malloc(n * sizeof(seq_t));
    if (p == NULL) abort();
    for (size_t i = 0; i < n; ++i) p[i] = 0;
    return p;
#endif
}

/* ********** Global static data (initialized once) ********** */
static bool initialized = false;
static size_t R;                     /* number of admissible row patterns */
static uint8_t (*row_vals)[7];       /* row_vals[i][p] is value at column p of row i */
static size_t total_states;          /* R * R */
static size_t *adj_start;            /* start index into adj_data for each state */
static size_t *adj_len;              /* number of outgoing edges for each state */
static uint16_t *adj_data;           /* concatenated destination row indices */

/* ********** Precompute row patterns and adjacency graph ********** */
static void precompute(void)
{
    if (initialized) return;
    initialized = true;

    /* 1. Enumerate all 7‑digit vectors over {0,1,2,3}. */
    const int CODE_MAX = 1 << 14;          /* 2 bits per element => 4^7 = 16384. */
    size_t tmpR = 0;
    uint8_t (*tmp_vals)[7] = (uint8_t (*)[7])malloc(CODE_MAX * 7 * sizeof(uint8_t));
    if (tmp_vals == NULL) abort();

    for (int code = 0; code < CODE_MAX; ++code) {
        int t = code;
        uint8_t v[7];
        for (int p = 0; p < 7; ++p) {
            v[p] = t & 3;
            t >>= 2;
        }
        bool ok = true;
        for (int j = 0; j <= 4; ++j) {
            int s = v[j] + v[j + 1] + v[j + 2];
            if (!(s == 3 || s == 6)) { ok = false; break; }
        }
        if (ok) {
            for (int p = 0; p < 7; ++p) tmp_vals[tmpR][p] = v[p];
            ++tmpR;
        }
    }

    /* Store final arrays. */
    R = tmpR;
    row_vals = tmp_vals;

    total_states = R * R;

    /* 2. Count outgoing edges for each state. */
    adj_len = (size_t *)malloc(total_states * sizeof(size_t));
    if (adj_len == NULL) abort();
    for (size_t i = 0; i < total_states; ++i) adj_len[i] = 0;

    for (size_t a = 0; a < R; ++a) {
        for (size_t b = 0; b < R; ++b) {
            size_t state = a * R + b;
            for (size_t c = 0; c < R; ++c) {
                bool ok = true;
                /* Column‑sum constraint: a[p]+b[p]+c[p] must be 3 or 6. */
                for (int p = 0; p < 7 && ok; ++p) {
                    int s = row_vals[a][p] + row_vals[b][p] + row_vals[c][p];
                    if (!(s == 3 || s == 6)) ok = false;
                }
                /* Diagonal constraints. */
                if (ok) {
                    for (int j = 0; j <= 4 && ok; ++j) {
                        int d1 = row_vals[a][j] + row_vals[b][j + 1] + row_vals[c][j + 2];
                        int d2 = row_vals[a][j + 2] + row_vals[b][j + 1] + row_vals[c][j];
                        if (d1 == 3 || d1 == 6 || d2 == 3 || d2 == 6) ok = false;
                    }
                }
                if (ok) adj_len[state]++;
            }
        }
    }

    /* 3. Allocate adjacency storage and fill it. */
    adj_start = (size_t *)malloc(total_states * sizeof(size_t));
    if (adj_start == NULL) abort();

    size_t total_edges = 0;
    for (size_t i = 0; i < total_states; ++i) {
        adj_start[i] = total_edges;
        total_edges += adj_len[i];
    }

    adj_data = (uint16_t *)malloc(total_edges * sizeof(uint16_t));
    if (adj_data == NULL) abort();

    for (size_t a = 0; a < R; ++a) {
        for (size_t b = 0; b < R; ++b) {
            size_t state = a * R + b;
            size_t pos = adj_start[state];
            for (size_t c = 0; c < R; ++c) {
                bool ok = true;
                for (int p = 0; p < 7 && ok; ++p) {
                    int s = row_vals[a][p] + row_vals[b][p] + row_vals[c][p];
                    if (!(s == 3 || s == 6)) ok = false;
                }
                if (ok) {
                    for (int j = 0; j <= 4 && ok; ++j) {
                        int d1 = row_vals[a][j] + row_vals[b][j + 1] + row_vals[c][j + 2];
                        int d2 = row_vals[a][j + 2] + row_vals[b][j + 1] + row_vals[c][j];
                        if (d1 == 3 || d1 == 6 || d2 == 3 || d2 == 6) ok = false;
                    }
                }
                if (ok) {
                    adj_data[pos++] = (uint16_t)c;
                }
            }
            /* pos now equals adj_start[state] + adj_len[state] */
        }
    }
}

/* ********** Main sequence function ********** */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1 */
    precompute();

    /* DP tables */
    size_t S = total_states;
    seq_t *dp_cur = seq_alloc(S);
    seq_t *dp_next = seq_alloc(S);

    /* Initial states: all pairs of admissible rows. */
    for (size_t i = 0; i < S; ++i) dp_cur[i] = 1;

    /* Perform n transitions (adding rows beyond the initial 2). */
    for (idx_t step = 0; step < n; ++step) {
        for (size_t i = 0; i < S; ++i) dp_next[i] = 0;

        for (size_t state = 0; state < S; ++state) {
            seq_t cur = dp_cur[state];
            if (cur == 0) continue;

            size_t a_row = state / R;
            size_t b_row = state % R;
            size_t start = adj_start[state];
            size_t len   = adj_len[state];
            for (size_t k = 0; k < len; ++k) {
                size_t c_row = (size_t)adj_data[start + k];
                size_t dest = b_row * R + c_row;
                dp_next[dest] += cur;
            }
        }
        /* Swap pointers */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum all final states */
    seq_t result = 0;
    for (size_t i = 0; i < S; ++i) result += dp_cur[i];

    /* Note: memory leaks are acceptable for short‑lived evaluation. */
    return result;
}
