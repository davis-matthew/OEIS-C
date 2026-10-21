/*
 * A282644  Number of n X 5 0..1 arrays with no 1 equal to more than one of
 *          its king-move neighbors.
 * https://oeis.org/A282644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A282644 – Number of n×5 binary arrays with at most one king‑move neighbour
 *  for every 1.  This is solved by dynamic programming over rows of width 5.
 *  The DP state is the last two rows; transitions are precomputed once
 *  into a bitmask table.  The implementation works for any build where
 *  seq_t is an unsigned integer type (8–128 bit or arbitrary precision)
 *  and idx_t is unsigned 64‑bit.  It compiles both as C and as C++17. */
#include <stdlib.h>
#include <stdint.h>

#define WIDTH 5
#define STATES (1U << WIDTH)   /* 32 possible rows */

static int is_valid_triple(int a, int b, int c)
{
    for (int col = 0; col < WIDTH; ++col) {
        int cnt = 0;
        /* neighbours from row a (above) */
        if (col > 0  && (a & (1 << (col-1)))) ++cnt;
        if (        a & (1 << (col)))          ++cnt;
        if (col < WIDTH-1 && (a & (1 << (col+1)))) ++cnt;
        /* neighbours from row c (below) */
        if (col > 0  && (c & (1 << (col-1)))) ++cnt;
        if (        c & (1 << (col)))          ++cnt;
        if (col < WIDTH-1 && (c & (1 << (col+1)))) ++cnt;
        /* neighbours left/right from row b (middle) */
        if (col > 0  && (b & (1 << (col-1)))) ++cnt;
        if (col < WIDTH-1 && (b & (1 << (col+1)))) ++cnt;
        /* cell itself must have at most one neighbour 1 */
        if ((b & (1 << col)) && cnt > 1)
            return 0;
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – unused */

    /* transition table: valid_next[a][b] is a 32‑bit mask where bit c
       is set if rows a,b,c form a valid triple. */
    static uint32_t valid_next[STATES][STATES];
    static int init = 0;
    if (!init) {
        for (int a = 0; a < STATES; ++a) {
            for (int b = 0; b < STATES; ++b) {
                uint32_t mask = 0;
                for (int c = 0; c < STATES; ++c) {
                    if (is_valid_triple(a, b, c))
                        mask |= 1U << c;
                }
                valid_next[a][b] = mask;
            }
        }
        init = 1;
    }

    const size_t dim = STATES * STATES;   /* DP array size */
    seq_t *dpCur;
    seq_t *dpNext;

#ifdef __cplusplus
    dpCur  = new seq_t[dim];
    dpNext = new seq_t[dim];
    for (size_t i = 0; i < dim; ++i) { dpCur[i] = 0; dpNext[i] = 0; }
#else
    dpCur  = (seq_t *)calloc(dim, sizeof(seq_t));
    dpNext = (seq_t *)calloc(dim, sizeof(seq_t));
    if (!dpCur || !dpNext) { free(dpCur); free(dpNext); return 0; }
#endif

    /* initial row – there is no row above, so prevprev = 0 */
    for (int cur = 0; cur < STATES; ++cur)
        dpCur[cur * STATES + 0] = 1;

    /* iterate over remaining rows */
    for (idx_t r = 1; r < n; ++r) {
        /* clear next DP array */
#ifdef __cplusplus
        for (size_t i = 0; i < dim; ++i) dpNext[i] = 0;
#else
        for (size_t i = 0; i < dim; ++i) dpNext[i] = 0;  /* calloc already zeroed first time */
#endif
        for (int prev = 0; prev < STATES; ++prev) {
            for (int prevprev = 0; prevprev < STATES; ++prevprev) {
                seq_t val = dpCur[prev * STATES + prevprev];
                if (!val) continue;
                uint32_t mask = valid_next[prevprev][prev];
                /* iterate over set bits of mask */
                while (mask) {
                    int cur = __builtin_ctz(mask);
                    mask &= mask - 1;
                    dpNext[cur * STATES + prev] += val;
                }
            }
        }
        /* swap current and next arrays */
        seq_t *tmp = dpCur;
        dpCur = dpNext;
        dpNext = tmp;
    }

    /* final validation – row below is a sentinel 0 */
    seq_t result = 0;
    for (int last = 0; last < STATES; ++last) {
        for (int prev = 0; prev < STATES; ++prev) {
            seq_t val = dpCur[last * STATES + prev];
            if (!val) continue;
            /* check transition from prev,last to 0 */
            if (valid_next[prev][last] & 1U)   /* bit 0 must be set */
                result += val;
        }
    }

#ifdef __cplusplus
    delete[] dpCur;
    delete[] dpNext;
#else
    free(dpCur);
    free(dpNext);
#endif

    return result;
}
