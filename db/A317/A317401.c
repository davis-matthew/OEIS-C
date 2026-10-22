/*
 * A317401  Number of distinct (up to rotation and reflection) tic-tac-toe
 *          positions with n tokens which allow a winning strategy for X.
 * https://oeis.org/A317401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute by exhaustive enumeration of all tic‑tac‑toe positions
 * (accounting for rotations and reflections) and a minimax solver.
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Board masks for the 8 winning lines (bits 0..8). */
static const uint32_t WIN_MASKS[8] = {
    0x00000007UL, /* row 0      0,1,2 */
    0x00000038UL, /* row 1      3,4,5 */
    0x000001C0UL, /* row 2      6,7,8 */
    0x00000049UL, /* col 0      0,3,6 */
    0x00000092UL, /* col 1      1,4,7 */
    0x00000124UL, /* col 2      2,5,8 */
    0x00000111UL, /* diag (0,0) 0,4,8 */
    0x00000054UL  /* diag (0,2) 2,4,6 */
};

/* Pre‑computed mapping of indices under the 8 dihedral transforms. */
static uint8_t TRANS[8][9];
static bool TRANS_INITIALIZED = false;
static void init_transforms(void)
{
    if (TRANS_INITIALIZED) return;
    for (int t = 0; t < 8; ++t) {
        int rot = t & 3;          /* rotation: 0‑3 (clockwise 90° steps) */
        int refl = (t >> 2) & 1;  /* reflection over vertical axis whether present */
        for (int idx = 0; idx < 9; ++idx) {
            int r = idx / 3;
            int c = idx % 3;
            if (refl) c = 2 - c;  /* apply vertical reflection first */
            for (int k = 0; k < rot; ++k) { /* then rotate 90° clockwise k times */
                int tmp = r;
                r = c;
                c = 2 - tmp;
            }
            TRANS[t][idx] = r * 3 + c;
        }
    }
    TRANS_INITIALIZED = true;
}

/* True if mask contains any winning line. */
static bool has_win(uint32_t mask)
{
    for (int i = 0; i < 8; ++i)
        if ((mask & WIN_MASKS[i]) == WIN_MASKS[i])
            return true;
    return false;
}

/* Solver cache: 127 = unknown, 1 = X wins, -1 = X cannot force win. */
static int8_t SOLVED[1 << 18];
static bool SOLVED_INITIALIZED = false;

/* Recursive minimax with memoisation. */
static int8_t solve(uint32_t key)
{
    if (SOLVED[key] != 127) return SOLVED[key];

    uint32_t xmask = key >> 9;
    uint32_t omask = key & 0x1FF;

    if (has_win(xmask)) return SOLVED[key] = 1;
    if (has_win(omask)) return SOLVED[key] = -1;
    if ((xmask | omask) == 0x1FF) return SOLVED[key] = -1;  /* full board */

    bool x_turn = (__builtin_popcount(xmask) == __builtin_popcount(omask));

    if (x_turn) {
        /* X’s turn: look for any winning move */
        for (int e = 0; e < 9; ++e) {
            if ((xmask | omask) & (1U << e)) continue; /* occupied */
            uint32_t new_key = ((xmask | (1U << e)) << 9) | omask;
            if (solve(new_key) == 1) return SOLVED[key] = 1;
        }
        return SOLVED[key] = -1;
    } else {
        /* O’s turn: try to prevent X’s win */
        for (int e = 0; e < 9; ++e) {
            if ((xmask | omask) & (1U << e)) continue;
            uint32_t new_key = (xmask << 9) | (omask | (1U << e));
            if (solve(new_key) == -1) return SOLVED[key] = -1;
        }
        return SOLVED[key] = 1;
    }
}

/* Count of winning positions for each token number (0..9). */
static seq_t COUNT[10];
static bool COUNT_INITIALIZED = false;
static uint8_t VISITED[1 << 18];

/* Enumerate all positions, canonise, solve, and count. */
static void compute_counts(void)
{
    if (COUNT_INITIALIZED) return;

    memset(COUNT, 0, sizeof(COUNT));
    memset(VISITED, 0, sizeof(VISITED));
    memset(SOLVED, 127, sizeof(SOLVED));

    init_transforms();

    const int TOTAL_STATES = 19683; /* 3^9 */

    for (int s = 0; s < TOTAL_STATES; ++s) {
        uint32_t xmask = 0, omask = 0;
        int tokens = 0;
        int tmp = s;
        for (int idx = 0; idx < 9; ++idx) {
            int val = tmp % 3;
            tmp /= 3;
            if (val == 1) { xmask |= 1U << idx; ++tokens; }
            else if (val == 2) { omask |= 1U << idx; ++tokens; }
        }

        int xcnt = __builtin_popcount(xmask);
        int ocnt = __builtin_popcount(omask);

        /* Valid official game positions only. */
        if (!((xcnt == ocnt) || (xcnt == ocnt + 1))) continue;
        if (has_win(xmask) && xcnt != ocnt + 1) continue;  /* X cannot win if counts equal */
        if (has_win(omask) && ocnt != xcnt) continue;      /* O cannot win if counts unequal */

        /* Reject positions with both having a line. */
        if (has_win(xmask) && has_win(omask)) continue;

        /* Canonicalise under D4 symmetries. */
        uint32_t min_key = UINT32_MAX;
        for (int t = 0; t < 8; ++t) {
            uint32_t nx = 0, no = 0;
            for (int idx = 0; idx < 9; ++idx) {
                if (xmask & (1U << idx)) nx |= 1U << TRANS[t][idx];
                if (omask & (1U << idx)) no |= 1U << TRANS[t][idx];
            }
            uint32_t key = (nx << 9) | no;
            if (key < min_key) min_key = key;
        }
        if (VISITED[min_key]) continue;
        VISITED[min_key] = 1;

        int8_t result = solve(min_key);
        if (result == 1) COUNT[tokens] += (seq_t)1;
    }

    COUNT_INITIALIZED = true;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    compute_counts();
    if (n < 10) return COUNT[n];
    return (seq_t)0;
}
