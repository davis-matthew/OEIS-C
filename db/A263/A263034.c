/*
 * A263034  Number of (n+2)X(2+2) 0..1 arrays with each row and column not
 *          divisible by 5, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A263034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   by dynamic programming over row patterns. a(n) = number of
 *            (n+2)×4 binary matrices whose every row and every column,
 *            interpreted as binary numbers with the most significant bit at
 *            the top/left, is not divisible by 5
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* binary row patterns (4 bits) not divisible by 5 */
static const uint8_t PATTERNS[12] = {
    0x1, 0x2, 0x3, 0x4,
    0x6, 0x7, 0x8, 0x9,
    0xB, 0xC, 0xD, 0xE
};

#define STATES 625            /* 5^4 possible column‑mod‑5 states */
static int TRANS[STATES][12]; /* s -> new state after adding a pattern */
static bool GOOD[STATES];     /* true if all four residues ≠ 0 */
static bool TRANS_READY = false;

/* pre‑compute TRANS and GOOD once */
static void init_transition(void)
{
    if (TRANS_READY) return;
    for (int s = 0; s < STATES; ++s) {
        int r[4];
        int tmp = s;
        for (int c = 0; c < 4; ++c) { r[c] = tmp % 5; tmp /= 5; }
        for (int p = 0; p < 12; ++p) {
            uint8_t pat = PATTERNS[p];
            int bit[4];
            bit[0] = (pat >> 3) & 1;     /* leftmost bit */
            bit[1] = (pat >> 2) & 1;
            bit[2] = (pat >> 1) & 1;
            bit[3] = pat & 1;            /* rightmost bit */
            int nr[4];
            for (int c = 0; c < 4; ++c)
                nr[c] = (r[c] * 2 + bit[c]) % 5;
            int ns = (((nr[0]*5)+nr[1])*5+nr[2])*5+nr[3];
            TRANS[s][p] = ns;
        }
        /* all residues non‑zero? */
        bool ok = true;
        tmp = s;
        for (int c = 0; c < 4; ++c) {
            int r0 = tmp % 5;
            if (r0 == 0) { ok = false; break; }
            tmp /= 5;
        }
        GOOD[s] = ok;
    }
    TRANS_READY = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n < 1 */

    init_transition();               /* ensure TRANS/GOOD ready */

    static seq_t dp[STATES];
    static seq_t next[STATES];

    /* initialise to zero */
    for (int s = 0; s < STATES; ++s) dp[s] = (seq_t)0;
    dp[0] = (seq_t)1;                /* start with all residues zero */

    int rows = (int)(n + 2);          /* number of rows to process */

    for (int row = 0; row < rows; ++row) {
        for (int s = 0; s < STATES; ++s) next[s] = (seq_t)0;
        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;
            for (int p = 0; p < 12; ++p) {
                int ns = TRANS[s][p];
                next[ns] = next[ns] + val;   /* seq_t addition */
            }
        }
        /* copy next -> dp for next iteration */
        for (int s = 0; s < STATES; ++s) dp[s] = next[s];
    }

    /* sum all states with all residues ≠ 0 */
    seq_t result = (seq_t)0;
    for (int s = 0; s < STATES; ++s)
        if (GOOD[s]) result += dp[s];

    return result;
}
