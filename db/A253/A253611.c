/*
 * A253611  Number of (n+2)X(1+2) 0..2 arrays with every consecutive three
 *          elements in every row and diagonal having one or two distinct
 *          values, and in every column and antidiagonal having two or three
 *          distinct values, and new values 0 upwards introduced in row major
 *          order.
 * https://oeis.org/A253611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   count of (n+2)×3 arrays over {0,1,2} with * - row and main
 *            diagonal triples having ≤2 distinct values, * - column and
 *            anti‑diagonal triples having ≥2 distinct values, * - “new values
 *            0 upwards introduced in row major order” (RGS)
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The DP works over the last two rows and the current maximum value. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Number of admissible 3‑cell row patterns (≤2 distinct values). */
#define P 21
/* Number of DP states: previous two row patterns and current maximum. */
#define S (P * P * 3)
/* Upper bound for the transition table size. */
#define MAX_TRANS (S * P)

/* Pattern storage */
static int patterns[P][3];
static int pattern_max[P];
/* Max value after appending pattern given starting maximum. */
static int nextmax_state[3][P];

/* Transition table: for each state, a flat list of destination states. */
static int trans_next[MAX_TRANS];
static int trans_offset[S];
static int trans_count[S];

/* DP arrays */
static seq_t *curr = NULL;
static seq_t *nxt  = NULL;

/* Ensure one‑time initialization. */
static void init_tables(void)
{
    /* Build admissible patterns. */
    int idx = 0;
    for (int a = 0; a <= 2; ++a)
        for (int b = 0; b <= 2; ++b)
            for (int c = 0; c <= 2; ++c) {
                if (a != b && a != c && b != c) continue;   /* exclude 3 distinct values */
                patterns[idx][0] = a;
                patterns[idx][1] = b;
                patterns[idx][2] = c;
                pattern_max[idx] = (a > b ? (a > c ? a : c) : (b > c ? b : c));
                ++idx;
            }
    /* idx should be 21. */

    /* Pre‑compute next maximum for each starting max and pattern. */
    for (int k = 0; k < 3; ++k) {
        for (int p = 0; p < P; ++p) {
            int cur = k;
            bool ok = true;
            for (int j = 0; j < 3; ++j) {
                int v = patterns[p][j];
                if (v > cur + 1) { ok = false; break; }
                if (v == cur + 1) cur = v;
            }
            nextmax_state[k][p] = ok ? cur : -1;
        }
    }

    /* Pre‑compute transition lists. */
    int total = 0;
    for (int s = 0; s < S; ++s) {
        int a = (s / 3) / P;
        int b = (s / 3) % P;
        int k = s % 3;
        const int *pa = patterns[a];
        const int *pb = patterns[b];
        trans_offset[s] = total;
        int cnt = 0;
        for (int c = 0; c < P; ++c) {
            int newk = nextmax_state[k][c];
            if (newk == -1) continue;
            const int *pc = patterns[c];
            /* Column constraint: no column triple all equal. */
            bool ok = true;
            for (int col = 0; col < 3; ++col) {
                if (pa[col] == pb[col] && pb[col] == pc[col]) { ok = false; break; }
            }
            if (!ok) continue;
            /* Diagonal constraint: main diagonal triple has ≤2 distinct values. */
            int x = pa[0], y = pb[1], z = pc[2];
            if (x != y && x != z && y != z) continue;
            /* Anti‑diagonal constraint: no triple all equal. */
            if (pa[2] == pb[1] && pb[1] == pc[0]) continue;
            int dst = ((b * P) + c) * 3 + newk;
            trans_next[total++] = dst;
            ++cnt;
        }
        trans_count[s] = cnt;
    }
    /* Allocate DP arrays once. */
    curr = (seq_t *)malloc(S * sizeof(seq_t));
    nxt  = (seq_t *)malloc(S * sizeof(seq_t));
}

/* --------------------------------------------------------------------- */
/* exported function                                               */
seq_t a(idx_t n)
{
    if (!curr || !nxt) init_tables();

    /* Clear DP arrays. */
    memset(curr, 0, S * sizeof(seq_t));
    memset(nxt,  0, S * sizeof(seq_t));

    /* --- initialise first two rows --- */
    for (int p0 = 0; p0 < P; ++p0) {
        int k1 = nextmax_state[0][p0];
        if (k1 == -1) continue;            /* invalid first row */
        const int *p0t = patterns[p0];
        /* first cell of first row must be 0 for RGS. */
        if (p0t[0] != 0) continue;
        for (int p1 = 0; p1 < P; ++p1) {
            int k2 = nextmax_state[k1][p1];
            if (k2 == -1) continue;
            int state = ((p0 * P) + p1) * 3 + k2;
            curr[state] += (seq_t)1;
        }
    }

    /* --- add remaining rows (n times) --- */
    for (idx_t step = 0; step < n; ++step) {
        memset(nxt, 0, S * sizeof(seq_t));
        for (int s = 0; s < S; ++s) {
            seq_t cnt = curr[s];
            if (cnt == 0) continue;
            int a = (s / 3) / P;
            int b = (s / 3) % P;
            int k = s % 3;
            const int *pa = patterns[a];
            const int *pb = patterns[b];
            for (int c = 0; c < P; ++c) {
                int newk = nextmax_state[k][c];
                if (newk == -1) continue;
                const int *pc = patterns[c];
                /* Column constraint */
                bool ok = true;
                for (int col = 0; col < 3; ++col) {
                    if (pa[col] == pb[col] && pb[col] == pc[col]) { ok = false; break; }
                }
                if (!ok) continue;
                /* Diagonal constraint */
                int x = pa[0], y = pb[1], z = pc[2];
                if (x != y && x != z && y != z) continue;
                /* Antidiagonal constraint */
                if (pa[2] == pb[1] && pb[1] == pc[0]) continue;
                int dst = ((b * P) + c) * 3 + newk;
                nxt[dst] += cnt;
            }
        }
        /* swap DP buffers */
        seq_t *tmp = curr; curr = nxt; nxt = tmp;
    }

    /* --- sum all final states --- */
    seq_t total = 0;
    for (int s = 0; s < S; ++s) total += curr[s];
    return total;
}
