/*
 * A282556  Number of n X 4 0..1 arrays with no 1 equal to more than two of
 *          its king-move neighbors, with the exception of exactly one
 *          element.
 * https://oeis.org/A282556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A282556: Number of n×4 0..1 arrays in which every 1 has at most two
    king‑move neighbours, except exactly one element, which may have more.
    The algorithm uses a transfer‑matrix DP over rows.  The DP state
    consists of the last two row patterns (previous and current) and
    whether a violating element has already been seen.  Only O(n) time
    and constant memory are required.  */

#include <stdint.h>            /* for uint64_t */
#include <stddef.h>           /* for size_t */

#define PATTERN_COUNT 16            /* 4 columns → 2^4 patterns */

/* Helper function to build all tables once */
static void build_tables(int hor[PATTERN_COUNT][4],
                         int abov[PATTERN_COUNT][4],
                         uint8_t bad_con[PATTERN_COUNT][PATTERN_COUNT][PATTERN_COUNT],
                         uint8_t fin_bad[PATTERN_COUNT][PATTERN_COUNT])
{
    /* horizontal neighbours inside the same row */
    for (int p = 0; p < PATTERN_COUNT; ++p) {
        for (int c = 0; c < 4; ++c) {
            int left  = (c > 0)   ? ((p >> (c-1)) & 1) : 0;
            int right = (c < 3)   ? ((p >> (c+1)) & 1) : 0;
            hor[p][c] = left + right;
        }
    }

    /* neighbours in the row just above (or below) */
    for (int p = 0; p < PATTERN_COUNT; ++p) {
        for (int c = 0; c < 4; ++c) {
            int up   = (p >> c) & 1;
            int ul   = (c > 0)   ? ((p >> (c-1)) & 1) : 0;
            int ur   = (c < 3)   ? ((p >> (c+1)) & 1) : 0;
            abov[p][c] = up + ul + ur;
        }
    }

    /* pre‑compute how many cells become bad when transitioning
       from (Q,P) to a new row C. */
    for (int q = 0; q < PATTERN_COUNT; ++q)
        for (int p = 0; p < PATTERN_COUNT; ++p)
            for (int c = 0; c < PATTERN_COUNT; ++c) {
                int cnt = 0;
                for (int col = 0; col < 4; ++col) {
                    if ((p >> col) & 1) {                      /* cell is 1   */
                        int neigh = hor[p][col] + abov[q][col] + abov[c][col];
                        if (neigh > 2) ++cnt;
                    }
                }
                bad_con[q][p][c] = (uint8_t)cnt;
            }

    /* pre‑compute the number of bad cells in the final row
       (no row below). */
    for (int q = 0; q < PATTERN_COUNT; ++q)
        for (int p = 0; p < PATTERN_COUNT; ++p) {
            int cnt = 0;
            for (int col = 0; col < 4; ++col) {
                if ((p >> col) & 1) {
                    int neigh = hor[p][col] + abov[q][col];
                    if (neigh > 2) ++cnt;
                }
            }
            fin_bad[q][p] = (uint8_t)cnt;
        }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */

    /* State index: ((Q<<4)+P)<<1 | bad  (bad ∈ {0,1})  */
    const size_t STATES = PATTERN_COUNT * PATTERN_COUNT * 2;
    static int hor;
    static int abov;
    static uint8_t bad_con;
    static uint8_t fin_bad;

    /* Tables allocated once */
    static int hor_tbl[PATTERN_COUNT][4] = {0};
    static int abov_tbl[PATTERN_COUNT][4] = {0};
    static uint8_t bad_con_tbl[PATTERN_COUNT][PATTERN_COUNT][PATTERN_COUNT] = {0};
    static uint8_t fin_bad_tbl[PATTERN_COUNT][PATTERN_COUNT] = {0};
    static int tables_built = 0;
    if (!tables_built) {
        build_tables(hor_tbl, abov_tbl, bad_con_tbl, fin_bad_tbl);
        tables_built = 1;
    }
    int (*H)[4] = hor_tbl;
    int (*A)[4] = abov_tbl;
    uint8_t (*BC)[PATTERN_COUNT][PATTERN_COUNT] = bad_con_tbl;
    uint8_t (*FB)[PATTERN_COUNT] = fin_bad_tbl;

    seq_t cur[STATES];
    seq_t nxt[STATES];
    for (size_t i = 0; i < STATES; ++i) cur[i] = 0;

    /* Initialise after first row: Q = 0 (no row above) */
    for (int p = 0; p < PATTERN_COUNT; ++p) {
        size_t idx = ((0 * PATTERN_COUNT + p) << 1) | 0;
        cur[idx] = 1;
    }

    /* Process all but the first row */
    for (idx_t row = 1; row < n; ++row) {
        for (size_t i = 0; i < STATES; ++i) nxt[i] = 0;
        for (int q = 0; q < PATTERN_COUNT; ++q)
            for (int p = 0; p < PATTERN_COUNT; ++p)
                for (int bad = 0; bad <= 1; ++bad) {
                    size_t idx = ((q * PATTERN_COUNT + p) << 1) | bad;
                    seq_t val = cur[idx];
                    if (!val) continue;
                    for (int c = 0; c < PATTERN_COUNT; ++c) {
                        int new_bad = bad + BC[q][p][c];
                        if (new_bad > 1) continue;
                        size_t nidx = (((p * PATTERN_COUNT) + c) << 1) | new_bad;
                        nxt[nidx] += val;
                    }
                }
        /* swap */
        for (size_t i = 0; i < STATES; ++i) cur[i] = nxt[i];
    }

    /* Final evaluation of the last row */
    seq_t result = 0;
    for (int q = 0; q < PATTERN_COUNT; ++q)
        for (int p = 0; p < PATTERN_COUNT; ++p)
            for (int bad = 0; bad <= 1; ++bad) {
                size_t idx = ((q * PATTERN_COUNT + p) << 1) | bad;
                seq_t val = cur[idx];
                if (!val) continue;
                int final_bad = bad + FB[q][p];
                if (final_bad == 1) result += val;
            }

    return result;
}
