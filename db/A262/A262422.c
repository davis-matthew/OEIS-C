/*
 * A262422  Number of (3+1) X (n+1) 0..1 arrays with each row divisible by 3
 *          and column not divisible by 3, read as a binary number with top
 *          and left being the most significant bits.
 * https://oeis.org/A262422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A262422: Number of (4)×(n+1) binary arrays such that each of the 4 rows
 * represents an integer divisible by 3 and each 4‑bit column represents
 * an integer NOT divisible by 3.  The array is read as a binary number
 * with the top‑left element as the most significant bit.
 *
 * Implementation: dynamic programming over columns.
 * State := residues modulo 3 of the 4 rows after processing the
 * current prefix.  A state is encoded as a base‑3 number 0‑80.
 * Transition: for each allowed 4‑bit pattern (those whose value
 * modulo 3 is non‑zero) the new residues are
 *   new_res = (old_res * 2 + bit) % 3   for each row.
 * After N = n+1 columns the count of ways with all residues = 0 is
 * the desired value.  The program precomputes the state decoding
 * and transition tables once.
 *
 * Initial values (computed by the above DP):
 * a(1)=10, a(2)=12, a(3)=270, a(4)=1260, a(5)=15310, … (see OEIS). */

#include <stdbool.h>

#define STATE_COUNT 81

static const unsigned char allowed_patterns[10] =
    {1,2,4,5,7,8,10,11,13,14};

static unsigned char decode[STATE_COUNT][4];
static unsigned char transition[STATE_COUNT][10];
static bool initialized = false;

static void init_tables(void)
{
    for (int idx = 0; idx < STATE_COUNT; ++idx) {
        int t = idx;
        decode[idx][0] = t / 27; t %= 27;
        decode[idx][1] = t / 9;  t %= 9;
        decode[idx][2] = t / 3;  t %= 3;
        decode[idx][3] = t;

        for (int pat = 0; pat < 10; ++pat) {
            unsigned char bit0 = (allowed_patterns[pat] >> 3) & 1;
            unsigned char bit1 = (allowed_patterns[pat] >> 2) & 1;
            unsigned char bit2 = (allowed_patterns[pat] >> 1) & 1;
            unsigned char bit3 = allowed_patterns[pat] & 1;

            unsigned char a0 = decode[idx][0];
            unsigned char a1 = decode[idx][1];
            unsigned char a2 = decode[idx][2];
            unsigned char a3 = decode[idx][3];

            unsigned char n0 = (a0 * 2 + bit0) % 3;
            unsigned char n1 = (a1 * 2 + bit1) % 3;
            unsigned char n2 = (a2 * 2 + bit2) % 3;
            unsigned char n3 = (a3 * 2 + bit3) % 3;

            transition[idx][pat] =
                ((n0 * 3 + n1) * 3 + n2) * 3 + n3;
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* below offset – undefined; return 0 */

    if (!initialized) init_tables();

    /* DP arrays: counts for each state */
    seq_t cur[STATE_COUNT] = {0};
    seq_t nxt[STATE_COUNT] = {0};

    cur[0] = (seq_t)1;   /* all row residues are 0 before any column */

    idx_t columns = n + 1;          /* N columns to process */
    for (idx_t col = 0; col < columns; ++col) {
        /* zero next array */
        for (int i = 0; i < STATE_COUNT; ++i) nxt[i] = (seq_t)0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = cur[s];
            if (val == (seq_t)0) continue;

            for (int p = 0; p < 10; ++p) {
                int ns = transition[s][p];
                nxt[ns] = nxt[ns] + val;
            }
        }

        /* copy nxt to cur for next iteration */
        for (int i = 0; i < STATE_COUNT; ++i) cur[i] = nxt[i];
    }

    return cur[0];  /* all row residues zero after all columns */
}
