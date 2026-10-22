/*
 * A392323  Number of ways to place non-attacking knights on a 4 X n
 *          chessboard.
 * https://oeis.org/A392323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is computed by a transfer‑matrix DP for a 4×n knight graph.
 *            State : 8 bits, lower 4 bits = occupancy of the previous column,
 *            upper 4 bits = occupancy of the column two steps back.
 *            Transition : a new column mask must not attack any knight in the
 *            previous column (at rows r±2) or in the column...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* types seq_t and idx_t are provided by the build environment */
#include <stdbool.h>

#define STATE_COUNT 256
#define MAX_NEW_MASK 16

static unsigned char trans[STATE_COUNT][MAX_NEW_MASK];
static unsigned char trans_count[STATE_COUNT] = {0};

static void init_transitions(void)
{
    for (unsigned int s = 0; s < STATE_COUNT; ++s) {
        unsigned char prev     = (unsigned char)(s & 0xF);           /* column n-1   */
        unsigned char prevprev = (unsigned char)((s >> 4) & 0xF);   /* column n-2   */

        /* conflicts from knights in column n-1 (distance 1): rows r±2 */
        unsigned char conflict1 = ((prev << 2) & 0xF) | ((prev >> 2) & 0xF);

        /* conflicts from knights in column n-2 (distance 2): rows r±1 */
        unsigned char conflict2 = ((prevprev << 1) & 0xF) | ((prevprev >> 1) & 0xF);

        unsigned char count = 0;
        for (unsigned char newMask = 0; newMask < MAX_NEW_MASK; ++newMask) {
            if ((newMask & conflict1) == 0 && (newMask & conflict2) == 0) {
                unsigned char newState = newMask | (prev << 4);
                trans[s][count++] = newState;
            }
        }
        trans_count[s] = count;
    }
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_transitions();
        initialized = true;
    }

    static seq_t dp[STATE_COUNT];
    static seq_t dp_next[STATE_COUNT];

    for (unsigned int i = 0; i < STATE_COUNT; ++i) dp[i] = 0;
    dp[0] = 1;  /* before first column, both previous columns are empty */

    for (idx_t col = 0; col < n; ++col) {
        for (unsigned int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        for (unsigned int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp[s];
            if (!val) continue;

            unsigned char* row   = trans[s];
            unsigned char rc     = trans_count[s];
            for (unsigned char j = 0; j < rc; ++j)
                dp_next[row[j]] += val;
        }

        for (unsigned int i = 0; i < STATE_COUNT; ++i) dp[i] = dp_next[i];
    }

    seq_t result = 0;
    for (unsigned int i = 0; i < STATE_COUNT; ++i) result += dp[i];
    return result;
}
