/*
 * A295209  Number of n X 4 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 2 or 3 1's.
 * https://oeis.org/A295209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A295209: Number of n×4 0..1 arrays with each 1 horizontally or
   vertically adjacent to 2 or 3 1's.  The algorithm uses a transfer‑matrix
   DP over rows.  The state of a row is encoded in 12 bits: for each of
   the 4 columns we store (bit<<2 | count_seen_before_down).  During a
   transition from a previous row state to a current row mask we check that
   all 1‑cells of the previous row reach a final neighbour count of 2 or
   3 after adding the vertical down neighbour.  After processing the last
   row we accept only states where each remaining 1‑cell already has a
   final count of 2 or 3 (no down neighbour).  The DP proceeds row by
   row; when a new row is added the number of completed arrays for that
   height is computed from the current dp table.  The function memoises
   results, so subsequent calls for different n are answered in
   O(n·states) time with a very small constant. */

#include <stdlib.h>
#include <stdint.h>

#define COLS 4
#define STATE_BITS (COLS * 3)   /* 12 bits per state */
#define MAX_STATE (1 << STATE_BITS)   /* 4096 states */

/* Transition tables (initialised once) */
static uint8_t  state_bit[MAX_STATE][COLS];
static uint8_t  state_cnt[MAX_STATE][COLS];
static uint8_t  trans_cnt[MAX_STATE];
static uint16_t trans_next[MAX_STATE][16];
static uint8_t  bottom_valid[MAX_STATE];
static int      tables_inited = 0;

static void init_tables(void)
{
    for (int s = 0; s < MAX_STATE; ++s) {
        int tmp = s;
        for (int j = 0; j < COLS; ++j) {
            uint8_t x = tmp & 0x7;
            state_bit[s][j] = x >> 2;
            state_cnt[s][j] = x & 0x3;
            tmp >>= 3;
        }

        /* Bottom-row validity (no down neighbour) */
        uint8_t ok = 1;
        for (int j = 0; j < COLS; ++j) {
            if (state_bit[s][j]) {
                uint8_t c = state_cnt[s][j];
                if (c != 2 && c != 3) { ok = 0; break; }
            }
        }
        bottom_valid[s] = ok;

        /* Precompute transitions */
        int cnt = 0;
        for (int cur = 0; cur < (1 << COLS); ++cur) {
            uint8_t valid = 1;
            for (int j = 0; j < COLS; ++j) {
                if (state_bit[s][j]) {
                    int final = state_cnt[s][j] + ((cur >> j) & 1);
                    if (final < 2 || final > 3) { valid = 0; break; }
                }
            }
            if (!valid) continue;

            int new_state = 0;
            for (int j = 0; j < COLS; ++j) {
                int up    = state_bit[s][j];
                int left  = (j > 0)    ? ((cur >> (j - 1)) & 1) : 0;
                int right = (j < COLS-1)? ((cur >> (j + 1)) & 1) : 0;
                int newCnt = (up ? 1 : 0) + left + right;
                int colBits = (((cur >> j) & 1) << 2) | newCnt;
                new_state |= colBits << (3 * j);
            }
            trans_next[s][cnt++] = new_state;
        }
        trans_cnt[s] = cnt;
    }
    tables_inited = 1;
}

/* DP memoisation data */
static seq_t *dp_cur = NULL, *dp_next = NULL;
static seq_t *memo   = NULL;
static idx_t memo_size   = 0;
static idx_t computed_max = 0;

seq_t a(idx_t n)
{
    /* Initialise transition tables once */
    if (!tables_inited) init_tables();

    /* Initialise DP memoisation structures once */
    if (!dp_cur) {
        dp_cur  = (seq_t*)calloc(MAX_STATE, sizeof(seq_t));
        dp_next = (seq_t*)calloc(MAX_STATE, sizeof(seq_t));
        /* start with an empty previous row (state 0) */
        dp_cur[0] = (seq_t)1;

        memo_size   = 1;      /* memo[0] unused */
        memo        = (seq_t*)calloc(memo_size, sizeof(seq_t));
        computed_max = 0;
    }

    /* Extend memo array if needed */
    if (n >= memo_size) {
        idx_t new_size = n + 1;
        seq_t *new_memo = (seq_t*)calloc(new_size, sizeof(seq_t));
        memcpy(new_memo, memo, memo_size * sizeof(seq_t));
        free(memo);
        memo = new_memo;
        memo_size = new_size;
    }

    /* If already computed, return */
    if (n <= computed_max)
        return memo[n];

    /* Run DP up to row n */
    for (idx_t row = computed_max + 1; row <= n; ++row) {
        /* Zero next dp array */
        for (int i = 0; i < MAX_STATE; ++i)
            dp_next[i] = (seq_t)0;

        /* Transition */
        for (int s = 0; s < MAX_STATE; ++s) {
            if (dp_cur[s] == (seq_t)0) continue;
            seq_t val = dp_cur[s];
            for (int t = 0; t < trans_cnt[s]; ++t) {
                int ns = trans_next[s][t];
                dp_next[ns] += val;
            }
        }

        /* Swap dp arrays */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        /* Compute a(row) from dp_cur (bottom row is finished) */
        seq_t total = (seq_t)0;
        for (int s = 0; s < MAX_STATE; ++s) {
            if (bottom_valid[s])
                total += dp_cur[s];
        }
        memo[row] = total;
    }

    computed_max = n;
    return memo[n];
}
