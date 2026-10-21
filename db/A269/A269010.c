/*
 * A269010  Number of nX7 binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A269010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by dynamic programming over rows, counting horizontal
 *            and * diagonal adjacency pairs in an n×7 binary array. Exactly
 *            one such pair * must sum to two
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Pre‑computation tables: horizontal adjacency within a row and transition
 * pairs between rows that add 0 or 1 more adjacency pairs. */
static bool init = false;
#define MASKS 128
static unsigned char horiz_adj[MASKS];
static unsigned char transition0[MASKS][MASKS];
static unsigned char transition1[MASKS][MASKS];
static size_t cnt0[MASKS], cnt1[MASKS];

static void precompute(void)
{
    for (int m = 0; m < MASKS; ++m) {
        unsigned int m_uint = (unsigned int)m;
        horiz_adj[m] = (unsigned char)__builtin_popcount(m_uint & (m_uint >> 1));
    }

    for (int p = 0; p < MASKS; ++p) {
        cnt0[p] = cnt1[p] = 0;
        for (int c = 0; c < MASKS; ++c) {
            unsigned int p_uint = (unsigned int)p;
            unsigned int c_uint = (unsigned int)c;
            unsigned int left  = (c_uint << 1) & 0x7F;
            unsigned int right = (c_uint >> 1);
            unsigned int cross = __builtin_popcount(p_uint & left)
                               + __builtin_popcount(p_uint & right);
            unsigned int e = horiz_adj[c] + cross;
            if (e == 0)
                transition0[p][cnt0[p]++] = (unsigned char)c;
            else if (e == 1)
                transition1[p][cnt1[p]++] = (unsigned char)c;
        }
    }
    init = true;
}

seq_t a(idx_t n)
{
    if (!init) precompute();

    /* DP arrays: dp[mask][k] counts ways ending with 'mask'
     * having exactly k adjacency pairs (k=0 or 1). */
    seq_t dp_arr[MASKS][2];
    seq_t next_arr[MASKS][2];
    seq_t (*cur)[2] = dp_arr;
    seq_t (*nxt)[2] = next_arr;

    /* Zero the arrays. */
    for (int i = 0; i < MASKS; ++i) {
        cur[i][0] = 0; cur[i][1] = 0;
        nxt[i][0] = 0; nxt[i][1] = 0;
    }

    /* First row initialization. */
    for (int m = 0; m < MASKS; ++m) {
        unsigned char h = horiz_adj[m];
        if (h <= 1) cur[m][h] = 1;
    }

    /* Process remaining rows. */
    for (idx_t row = 2; row <= n; ++row) {
        /* Zero the next array. */
        for (int i = 0; i < MASKS; ++i) {
            nxt[i][0] = 0; nxt[i][1] = 0;
        }

        for (int p = 0; p < MASKS; ++p) {
            seq_t val0 = cur[p][0];
            seq_t val1 = cur[p][1];

            if (val0) {
                /* Transition with extra 0 adjacency. */
                for (size_t t = 0; t < cnt0[p]; ++t) {
                    unsigned char c = transition0[p][t];
                    nxt[c][0] += val0;
                }
                /* Transition with extra 1 adjacency. */
                for (size_t t = 0; t < cnt1[p]; ++t) {
                    unsigned char c = transition1[p][t];
                    nxt[c][1] += val0;
                }
            }
            if (val1) {
                /* Only extra 0 adjacency possible, moves to k=1. */
                for (size_t t = 0; t < cnt0[p]; ++t) {
                    unsigned char c = transition0[p][t];
                    nxt[c][1] += val1;
                }
            }
        }

        /* Swap current and next arrays. */
        seq_t (*tmp)[2] = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all ways that have exactly one adjacency pair. */
    seq_t result = 0;
    for (int m = 0; m < MASKS; ++m) {
        result += cur[m][1];
    }
    return result;
}
