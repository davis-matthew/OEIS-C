/*
 * A262237  Number of (n+3)X(2+3) 0..1 arrays with each row and column
 *          divisible by 11, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A262237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A262237: Number of (n+3)×(5) binary arrays with each
 * row and each column interpreted as a binary number divisible by 11.
 * The array is read in row-major order with the most significant
 * bit at the top-left corner.
 *
 * The computation uses dynamic programming over the
 * remainders modulo 11 of the three distinct column types:
 *  - Type A: columns 0 and 2 (both receive the same bit in each row)
 *  - Type B: columns 1 and 4 (both receive the same bit in each row)
 *  - Type C: column 3
 *
 * For each of the three possible row patterns
 *   P0 = 00000  → (A,B,C) = (0,0,0)
 *   P1 = 01011  → (A,B,C) = (0,1,1)
 *   P2 = 10110  → (A,B,C) = (1,0,1)
 * the remainder of each column type is updated as
 *   r' = (2*r + bit) mod 11
 *
 * A state is represented by the triple (rA,rB,rC), 0 ≤ r < 11,
 * encoded as an integer s = rA*121 + rB*11 + rC (range 0..1330).
 *
 * DP iterates over the L=n+3 rows; after processing all rows
 * the number of valid arrays is the number of ways to return to
 * state (0,0,0).  The algorithm runs in O(L·11³) ≈ 4·10⁴
 * operations per call, which is far below the 2‑second limit.
 */

#include <stddef.h>
#include <stdint.h>

static const int STATE_SIZE = 1331;          // 11^3
static const int MOD = 11;

/* Bits for each column type for the three row patterns */
static const unsigned char A_bits[3] = {0, 0, 1};
static const unsigned char B_bits[3] = {0, 1, 0};
static const unsigned char C_bits[3] = {0, 1, 1};

seq_t a(idx_t n) {
    /* Number of rows */
    idx_t L = n + 3;

    /* Allocate DP arrays */
    seq_t *dp_cur = (seq_t*)calloc(STATE_SIZE, sizeof(seq_t));
    seq_t *dp_next = (seq_t*)calloc(STATE_SIZE, sizeof(seq_t));
    if (!dp_cur || !dp_next) return (seq_t)0; /* In practice this won't happen */

    /* Initial state: all remainders 0 */
    dp_cur[0] = (seq_t)1;

    for (idx_t step = 0; step < L; ++step) {
        /* Clear next DP array */
        for (int i = 0; i < STATE_SIZE; ++i) dp_next[i] = 0;

        /* Iterate over all states */
        for (int s = 0; s < STATE_SIZE; ++s) {
            seq_t cur_val = dp_cur[s];
            if (cur_val == 0) continue;

            /* Decode remainders */
            int rA = s / (MOD * MOD);
            int rem = s % (MOD * MOD);
            int rB = rem / MOD;
            int rC = rem % MOD;

            /* For each row pattern */
            for (int pat = 0; pat < 3; ++pat) {
                int nA = (rA * 2 + A_bits[pat]) % MOD;
                int nB = (rB * 2 + B_bits[pat]) % MOD;
                int nC = (rC * 2 + C_bits[pat]) % MOD;
                int ns = nA * (MOD * MOD) + nB * MOD + nC;
                dp_next[ns] += cur_val;
            }
        }

        /* Swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    seq_t result = dp_cur[0];
    free(dp_cur);
    free(dp_next);
    return result;
}
