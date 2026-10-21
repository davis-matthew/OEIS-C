/*
 * A251296  Number of (n+1)X(4+1) 0..3 arrays with every 2X2 subblock summing
 *          to 3 6 or 9.
 * https://oeis.org/A251296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming over compatible row transitions.
   The number of (n+1)×5 arrays with entries 0..3 such that every 2×2
   subblock sums to 3, 6 or 9. */

#include <stdlib.h>
#include <string.h>

/* Global transition tables, built once. */
static unsigned short *a_edges = NULL;
static size_t *a_offset = NULL;

/* Build the transition graph for all 5‑digit rows over {0,1,2,3}. */
static void build_transitions(void)
{
    static unsigned char digits[1024][5];
    /* Precompute the base‑4 digits of every state. */
    for (int s = 0; s < 1024; ++s) {
        for (int j = 0; j < 5; ++j)
            digits[s][j] = (s >> (2 * j)) & 0x3;
    }

    /* Count how many successors each state has. */
    int outdeg[1024] = {0};
    for (int p = 0; p < 1024; ++p) {
        for (int q = 0; q < 1024; ++q) {
            int ok = 1;
            for (int j = 0; j < 4; ++j) {
                int sum = digits[p][j] + digits[p][j + 1] +
                          digits[q][j] + digits[q][j + 1];
                if (sum != 3 && sum != 6 && sum != 9) {
                    ok = 0; break;
                }
            }
            if (ok) ++outdeg[p];
        }
    }

    /* Total number of edges. */
    size_t total_edges = 0;
    for (int p = 0; p < 1024; ++p) total_edges += outdeg[p];

    /* Allocate arrays. */
    unsigned short *edges  = (unsigned short *)malloc(total_edges *
                                                      sizeof *edges);
    size_t *offset = (size_t *)malloc(1025 * sizeof *offset);

    /* Set offsets and reuse outdeg as current write index. */
    offset[0] = 0;
    for (int p = 0; p < 1024; ++p) {
        offset[p + 1] = offset[p] + outdeg[p];
        outdeg[p] = 0;          /* reset to zero for writing */
    }

    /* Fill the edge list. */
    for (int p = 0; p < 1024; ++p) {
        for (int q = 0; q < 1024; ++q) {
            int ok = 1;
            for (int j = 0; j < 4; ++j) {
                int sum = digits[p][j] + digits[p][j + 1] +
                          digits[q][j] + digits[q][j + 1];
                if (sum != 3 && sum != 6 && sum != 9) {
                    ok = 0; break;
                }
            }
            if (ok) {
                edges[offset[p] + outdeg[p]++] = (unsigned short)q;
            }
        }
    }

    /* Store pointers for use by a(). */
    a_edges   = edges;
    a_offset  = offset;
}

/* The sequence function. */
seq_t a(idx_t n)
{
    if (a_edges == NULL) build_transitions();

    /* DP buffers. */
    static seq_t buf0[1024];
    static seq_t buf1[1024];
    seq_t *curr  = buf0;
    seq_t *next  = buf1;

    /* Initial row: all 1024 patterns are possible, count 1 each. */
    for (int i = 0; i < 1024; ++i) curr[i] = 1;

    for (idx_t step = 0; step < n; ++step) {
        /* Zero the next row buffer. */
        memset(next, 0, 1024 * sizeof(seq_t));
        /* Transition counts. */
        for (int p = 0; p < 1024; ++p) {
            seq_t val = curr[p];
            if (val == 0) continue;
            size_t start = a_offset[p];
            size_t end   = a_offset[p + 1];
            for (size_t k = start; k < end; ++k) {
                unsigned short q = a_edges[k];
                next[q] += val;
            }
        }
        /* Swap buffers. */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    /* Sum counts of all final rows. */
    seq_t result = 0;
    for (int i = 0; i < 1024; ++i) result += curr[i];
    return result;
}
