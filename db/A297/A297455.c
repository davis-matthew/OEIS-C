/*
 * A297455  Number of nX6 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0 or 2 neighboring 1s.
 * https://oeis.org/A297455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence A297455 is computed by a 2‑row transfer‑matrix DP. The
 *            state is the last two rows (prevprev, prev); the transition
 *            enumerates all possible new rows (curr) such that row 'prev'
 *            satisfies the 0‑or‑2 neighbor rule with neighbors from prevprev
 *            and curr
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 0/1 arrays of width 6.  Horizontal left/right and the two diagonals
   (NW–SE and NE–SW) are considered; vertical adjacency is ignored. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper to allocate an array of seq_t of given length,
   compatible with both C11 and C++17. */
static seq_t *alloc_array(idx_t len)
{
#ifdef __cplusplus
    /* C++: use new[] which value‑initializes builtin types and calls
       constructors of class types.  For built‑in numeric types the array
       elements are automatically zeroed. */
    seq_t *p = new seq_t[len];
#else
    /* C: use malloc and zero the memory explicitly. */
    seq_t *p = (seq_t *)malloc(sizeof(seq_t) * len);
    if (p) memset(p, 0, sizeof(seq_t) * len);
#endif
    return p;
}

/* Width of the grid. */
#define WIDTH 6
#define NSTATE (1 << WIDTH)

/* Bit masks for neighbour shifts. */
static const uint8_t LEFT_MASK  = 0x7E; /* 0b01111110, bits 1..5 set */
static const uint8_t RIGHT_MASK = 0x7E; /* same, will be ANDed after shift */

/* Transition table: trans[prevprev][prev] is a list of curr patterns
   that make row 'prev' valid.  We store the list in a static array and
   its length in trans_len.  The table is initialized on the first call. */
static uint8_t trans[NSTATE][NSTATE][NSTATE];
static uint8_t trans_len[NSTATE][NSTATE];
static bool trans_filled = false;

/* Test whether row 'prev' satisfies the rule using the patterns
   prevprev (row above) and curr (row below). */
static inline bool row_is_valid(uint8_t prevprev, uint8_t prev, uint8_t curr)
{
    for (int c = 0; c < WIDTH; ++c) {
        if (!(prev & (1u << c))) continue;       /* skip 0 cells */

        int deg = 0;
        /* left neighbour in same row */
        if (c > 0 && (prev & (1u << (c - 1)))) deg++;
        /* right neighbour in same row */
        if (c < WIDTH - 1 && (prev & (1u << (c + 1)))) deg++;
        /* up-left neighbour (prevprev) */
        if (c > 0 && (prevprev & (1u << (c - 1)))) deg++;
        /* up-right neighbour (prevprev) */
        if (c < WIDTH - 1 && (prevprev & (1u << (c + 1)))) deg++;
        /* down-left neighbour (curr) */
        if (c > 0 && (curr & (1u << (c - 1)))) deg++;
        /* down-right neighbour (curr) */
        if (c < WIDTH - 1 && (curr & (1u << (c + 1)))) deg++;

        if (deg != 0 && deg != 2) return false;
    }
    return true;
}

/* Fill the transition table only once. */
static void fill_transitions(void)
{
    if (trans_filled) return;
    for (int pp = 0; pp < NSTATE; ++pp) {
        for (int p = 0; p < NSTATE; ++p) {
            uint8_t *list = trans[pp][p];
            uint8_t count = 0;
            for (int c = 0; c < NSTATE; ++c) {
                if (row_is_valid((uint8_t)pp, (uint8_t)p, (uint8_t)c)) {
                    list[count++] = (uint8_t)c;
                }
            }
            trans_len[pp][p] = count;
        }
    }
    trans_filled = true;
}

/* Memoised cache of computed terms. */
static seq_t *term_cache = NULL;
static idx_t cache_len   = 0;

/* Main function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* value for offset 0 is irrelevant */

    /* Ensure transitions are available. */
    fill_transitions();

    /* Resize cache if needed. */
    if (n > cache_len) {
        /* Allocate new cache large enough for indices 0..n. */
        seq_t *new_cache = alloc_array((size_t)n + 1);
        if (!new_cache) return 0; /* allocation failed (unlikely) */

        /* Zero‑initialize the cache. */
        for (idx_t i = 0; i <= n; ++i) new_cache[i] = 0;

        /* DP arrays: dpPrev[prevprev][prev] holds counts after a row
           has been added.  dpCurr is temporary.  They are kept on the
           stack because the size is moderate. */
        static seq_t dpPrev[NSTATE][NSTATE];
        static seq_t dpCurr[NSTATE][NSTATE];

        /* Clear dpPrev and set up for the first real row (row 0). */
        for (int i = 0; i < NSTATE; ++i)
            for (int j = 0; j < NSTATE; ++j)
                dpPrev[i][j] = 0;
        for (int p = 0; p < NSTATE; ++p)
            dpPrev[0][p] = 1;      /* top dummy row is 0 */

        /* Compute a(1): finalize row 0 by adding a bottom dummy row. */
        seq_t sum = 0;
        for (int prev = 0; prev < NSTATE; ++prev) {
            if (row_is_valid(0, (uint8_t)prev, 0))
                sum += dpPrev[0][prev];
        }
        new_cache[1] = sum;

        /* Iteratively add rows 2..n.  After each addition we compute
           the answer for that n by finalizing with a bottom dummy row. */
        for (int row = 2; row <= (int)n; ++row) {
            /* Clear dpCurr. */
            for (int i = 0; i < NSTATE; ++i)
                for (int j = 0; j < NSTATE; ++j)
                    dpCurr[i][j] = 0;

            /* Process transitions. */
            for (int pp = 0; pp < NSTATE; ++pp) {
                for (int p = 0; p < NSTATE; ++p) {
                    seq_t val = dpPrev[pp][p];
                    if (val == 0) continue;
                    uint8_t *list = trans[pp][p];
                    uint8_t len   = trans_len[pp][p];
                    for (uint8_t k = 0; k < len; ++k) {
                        int c = list[k];
                        dpCurr[p][c] += val;
                    }
                }
            }

            /* Swap dpPrev and dpCurr (copy dpCurr into dpPrev). */
            for (int i = 0; i < NSTATE; ++i)
                for (int j = 0; j < NSTATE; ++j)
                    dpPrev[i][j] = dpCurr[i][j];

            /* Compute a(row) by finalizing the last real row. */
            seq_t ans = 0;
            for (int pp = 0; pp < NSTATE; ++pp) {
                for (int p = 0; p < NSTATE; ++p) {
                    seq_t cnt = dpPrev[pp][p];
                    if (cnt == 0) continue;
                    if (row_is_valid(pp, (uint8_t)p, 0))
                        ans += cnt;
                }
            }
            new_cache[row] = ans;
        }

        /* Free old cache if present. */
        if (term_cache != NULL) {
#ifdef __cplusplus
            delete[] term_cache;
#else
            free(term_cache);
#endif
        }

        term_cache = new_cache;
        cache_len  = n;
    }

    return term_cache[n];
}
