/*
 * A263114  Number of (n+2)X(2+2) 0..1 arrays with each row and column not
 *          divisible by 7, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A263114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence style):
   a(n) = number of (n+2)×4 binary matrices whose rows and
   columns, read as binary numbers (top row / leftmost column
   most significant), are not divisible by 7.
   Only the rows are required to be admissible (value % 7 != 0);
   then a linear‑time DP over the 7‑ary column remainders is used. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* 13 admissible 4‑bit row patterns (their integer value % 7 != 0) */
static const uint8_t PATTERNS[13] = {
    1, 2, 3, 4, 5, 6, 8, 9,10,11,12,13,15
};

enum { STATES = 2401 };                /* 7⁴ */
static bool TRANS_INIT = false;
static uint16_t TRANS[STATES][13];      /* state transition table */

/* DP tables for each step */
static seq_t DP[STATES];
static seq_t NEXT[STATES];

/* Helper to build the transition table once. */
static void build_transition(void)
{
    if (TRANS_INIT) return;
    unsigned r0, r1, r2, r3, state = 0;
    for (r0 = 0; r0 < 7; ++r0)
    for (r1 = 0; r1 < 7; ++r1)
    for (r2 = 0; r2 < 7; ++r2)
    for (r3 = 0; r3 < 7; ++r3) {
        unsigned p;
        for (p = 0; p < 13; ++p) {
            uint8_t val = PATTERNS[p];
            unsigned bit0 = (val >> 3) & 1;
            unsigned bit1 = (val >> 2) & 1;
            unsigned bit2 = (val >> 1) & 1;
            unsigned bit3 = val & 1;

            unsigned nr0 = (r0 * 2 + bit0) % 7;
            unsigned nr1 = (r1 * 2 + bit1) % 7;
            unsigned nr2 = (r2 * 2 + bit2) % 7;
            unsigned nr3 = (r3 * 2 + bit3) % 7;

            TRANS[state][p] = (uint16_t)(
                (((nr0 * 7 + nr1) * 7 + nr2) * 7 + nr3));
        }
        ++state;
    }
    TRANS_INIT = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1 */

    build_transition();

    /* number of rows = n + 2 */
    idx_t rows = n + 2;

    /* initialise dp */
    {
        unsigned i;
        for (i = 0; i < STATES; ++i) DP[i] = 0;
        DP[0] = (seq_t)1;
    }

    /* temporary pointers for DP arrays */
    seq_t *cur  = DP;
    seq_t *next = NEXT;

    idx_t step;
    for (step = 0; step < rows; ++step) {
        unsigned i, p;
        /* zero next */
        for (i = 0; i < STATES; ++i) next[i] = 0;

        /* transition */
        for (i = 0; i < STATES; ++i) {
            seq_t cnt = cur[i];
            if (cnt == 0) continue;
            for (p = 0; p < 13; ++p) {
                unsigned ns = TRANS[i][p];
                next[ns] += cnt;
            }
        }

        /* swap */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum states where all column remainders are non‑zero */
    seq_t result = 0;
    unsigned i;
    for (i = 0; i < STATES; ++i) {
        seq_t cnt = cur[i];
        if (cnt == 0) continue;

        unsigned rem = i;
        unsigned r3 = rem % 7; rem /= 7;
        unsigned r2 = rem % 7; rem /= 7;
        unsigned r1 = rem % 7; rem /= 7;
        unsigned r0 = rem;            /* most significant part */

        if (r0 && r1 && r2 && r3) {
            result += cnt;
        }
    }
    return result;
}
