/*
 * A282643  Number of nX4 0..1 arrays with no 1 equal to more than one of its
 *          king-move neighbors.
 * https://oeis.org/A282643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1)+16*a(n-2)+45*a(n-3)+16*a(n-4)
                    -2*a(n-5)-51*a(n-6)-35*a(n-7)-23*a(n-8)+7*a(n-9).
   Initial values: a(1)=13,a(2)=76,a(3)=413,a(4)=2638,a(5)=15460,
                   a(6)=92817,a(7)=557439,a(8)=3332685,a(9)=19979228. */

#include <stddef.h>
#include <stdbool.h>

#define NUM_STATES 81   /* 3^4 possible column states */
#define NUM_COLS   4
#define NUM_PATTERNS 16 /* 2^4 possible row patterns */

static bool          trans_ok   [NUM_STATES][NUM_PATTERNS];
static unsigned char trans_next[NUM_STATES][NUM_PATTERNS];
static bool          trans_inited = false;

/* Precompute the transition table once. */
static void init_trans(void)
{
    for (int s = 0; s < NUM_STATES; ++s) {
        /* Decode state s into its 4 base‑3 digits (0,1,2). */
        int digits[NUM_COLS];
        int tmp = s;
        for (int j = 0; j < NUM_COLS; ++j) {
            digits[j] = tmp % 3;
            tmp /= 3;
        }

        for (int r = 0; r < NUM_PATTERNS; ++r) {
            int cur_bit[NUM_COLS];
            for (int j = 0; j < NUM_COLS; ++j)
                cur_bit[j] = (r >> j) & 1;

            bool ok = true;

            /* 1. Check constraint for each previous row cell. */
            for (int j = 0; j < NUM_COLS && ok; ++j) {
                if (digits[j] == 2) {               /* already has a neighbor */
                    for (int k = j - 1; k <= j + 1 && ok; ++k)
                        if (k >= 0 && k < NUM_COLS && cur_bit[k])
                            ok = false;
                } else if (digits[j] == 1) {        /* still free for one neighbour */
                    int cnt = 0;
                    for (int k = j - 1; k <= j + 1; ++k)
                        if (k >= 0 && k < NUM_COLS && cur_bit[k]) {
                            ++cnt;
                            if (cnt > 1) { ok = false; break; }
                        }
                }
            }

            if (!ok) {
                trans_ok[s][r] = false;
                continue;
            }

            /* 2. Compute next state for current row cells. */
            int next_digits[NUM_COLS];
            for (int j = 0; j < NUM_COLS && ok; ++j) {
                if (!cur_bit[j]) {
                    next_digits[j] = 0;
                    continue;
                }

                int neigh = 0;
                /* neighbors from the previous row (vertical + diagonals) */
                if (j > 0 && digits[j - 1] != 0) ++neigh;
                if (digits[j] != 0)                 ++neigh;
                if (j < NUM_COLS - 1 && digits[j + 1] != 0) ++neigh;

                /* horizontal neighbors in current row */
                if (j > 0 && cur_bit[j - 1]) ++neigh;
                if (j < NUM_COLS - 1 && cur_bit[j + 1]) ++neigh;

                if (neigh > 1) { ok = false; break; }

                next_digits[j] = (neigh >= 1) ? 2 : 1;
            }

            if (!ok) {
                trans_ok[s][r] = false;
                continue;
            }

            /* encode next state */
            int enc = 0;
            int mul = 1;
            for (int j = 0; j < NUM_COLS; ++j) {
                enc += next_digits[j] * mul;
                mul *= 3;
            }
            trans_ok[s][r] = true;
            trans_next[s][r] = (unsigned char)enc;
        }
    }
    trans_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* sequence defined for n >= 1 */

    if (!trans_inited)
        init_trans();

    seq_t cur[NUM_STATES];
    seq_t nxt[NUM_STATES];
    for (int i = 0; i < NUM_STATES; ++i) {
        cur[i] = 0;
        nxt[i] = 0;
    }
    cur[0] = 1;            /* start with all columns empty */

    for (idx_t row = 0; row < n; ++row) {
        for (int i = 0; i < NUM_STATES; ++i)
            nxt[i] = 0;

        for (int s = 0; s < NUM_STATES; ++s) {
            if (cur[s] == (seq_t)0)
                continue;
            for (int r = 0; r < NUM_PATTERNS; ++r) {
                if (!trans_ok[s][r])
                    continue;
                int ns = trans_next[s][r];
                nxt[ns] += cur[s];
            }
        }

        for (int i = 0; i < NUM_STATES; ++i)
            cur[i] = nxt[i];
    }

    seq_t ans = 0;
    for (int i = 0; i < NUM_STATES; ++i)
        ans += cur[i];

    return ans;
}
