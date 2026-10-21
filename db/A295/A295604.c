/*
 * A295604  Number of nX6 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 2, 3 or 4 1s.
 * https://oeis.org/A295604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A295604: transfer‑matrix DP for counting n×6 binary arrays in which no
   1 has exactly one adjacent 1 (horizontally or vertically).  */
#include <stdlib.h>
#include <stdbool.h>

#define COLS   6
#define STATES 4096        /* 4^6 states */

static short *trans = NULL;   /* [state * 64 + r] -> next state or -1 */
static bool *valid_last = NULL;/* final state legality flag */

/* encode 6 base‑4 digits (most significant first) into a state id */
static inline int encode_state(const int code[COLS]) {
    int id = 0;
    for (int i = 0; i < COLS; ++i) id = id * 4 + code[i];
    return id;
}

/* initialise transition tables once – lazy caling from a() */
static void init_tables(void) {
    static bool inited = false;
    if (inited) return;
    inited = true;

    trans = (short*)malloc(sizeof(short) * STATES * 64);
    valid_last = (bool*)malloc(sizeof(bool) * STATES);
    for (int i = 0; i < STATES * 64; ++i) trans[i] = -1;

    for (int s = 0; s < STATES; ++s) {
        int tmp = s;
        int code_prev[COLS];
        int bit_prev[COLS];
        for (int i = COLS - 1; i >= 0; --i) {
            code_prev[i] = tmp % 4;
            tmp /= 4;
            bit_prev[i] = (code_prev[i] > 0);
        }

        /* final row must not contain a 1 with a partial degree of 1 */
        bool ok = true;
        for (int i = 0; i < COLS; ++i)
            if (code_prev[i] == 2) { ok = false; break; }
        valid_last[s] = ok;

        /* transitions for all 64 possible next row patterns */
        for (int r = 0; r < 64; ++r) {
            bool row_ok = true;
            for (int j = 0; j < COLS; ++j) {
                if (!bit_prev[j]) continue;
                int below = (r >> j) & 1;
                /* code 1 -> 0 neighbors so far → require below==0
                   code 2 -> 1 neighbor so far → require below==1
                   code 3 -> already ≥2 neighbors → any below allowed   */
                if (code_prev[j] == 1 && below == 1) { row_ok = false; break; }
                if (code_prev[j] == 2 && below == 0) { row_ok = false; break; }
            }
            if (!row_ok) continue;

            int code_cur[COLS];
            for (int j = 0; j < COLS; ++j) {
                int rbit   = (r >> j) & 1;
                int hdeg   = 0;
                if (j > 0     && (r >> (j-1)) & 1) hdeg++;
                if (j < COLS-1 && (r >> (j+1)) & 1) hdeg++;
                int vdeg   = bit_prev[j];
                int deg    = hdeg + vdeg;   /* 0..3 */

                if (rbit == 0)          code_cur[j] = 0;
                else if (deg == 0)      code_cur[j] = 1;
                else if (deg == 1)      code_cur[j] = 2;
                else                    code_cur[j] = 3;
            }
            int ns = encode_state(code_cur);
            trans[s * 64 + r] = (short)ns;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    init_tables();

    static seq_t *dp    = NULL;
    static seq_t *newdp = NULL;
    if (!dp) {
        dp    = (seq_t*)malloc(sizeof(seq_t) * STATES);
        newdp = (seq_t*)malloc(sizeof(seq_t) * STATES);
    }

    /* first row */
    for (int i = 0; i < STATES; ++i) dp[i] = (seq_t)0;
    for (int r = 0; r < 64; ++r) {
        int code_cur[COLS];
        for (int j = 0; j < COLS; ++j) {
            int rbit = (r >> j) & 1;
            int hdeg = 0;
            if (j > 0     && (r >> (j-1)) & 1) hdeg++;
            if (j < COLS-1 && (r >> (j+1)) & 1) hdeg++;
            int deg = hdeg;   /* no vertical neighbour yet */

            if (rbit == 0)          code_cur[j] = 0;
            else if (deg == 0)      code_cur[j] = 1;
            else if (deg == 1)      code_cur[j] = 2;
            else                    code_cur[j] = 3;
        }
        int ns = encode_state(code_cur);
        dp[ns] += (seq_t)1;
    }

    /* remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < STATES; ++i) newdp[i] = (seq_t)0;
        for (int s = 0; s < STATES; ++s) {
            if (dp[s] == (seq_t)0) continue;
            for (int r = 0; r < 64; ++r) {
                int ns = trans[s * 64 + r];
                if (ns >= 0) newdp[ns] += dp[s];
            }
        }
        seq_t *tmp = dp; dp = newdp; newdp = tmp;
    }

    /* sum of valid terminal states */
    seq_t result = (seq_t)0;
    for (int s = 0; s < STATES; ++s)
        if (valid_last[s]) result += dp[s];

    return result;
}
