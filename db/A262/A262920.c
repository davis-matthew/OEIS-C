/*
 * A262920  Number of (6+1)X(n+1) 0..1 arrays with each row divisible by 3 and
 *          each column divisible by 7, read as a binary number with top and
 *          left being the most significant bits.
 * https://oeis.org/A262920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computation of A262920 via dynamic programming over
 *            row‐remainders * (mod 3) and column vectors satisfying mod 7
 *            constraints
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* (n+1) columns, 7 rows */
#define ROWS 7
#define STATE_COUNT (2187)            /* 3^7 */
#define MAX_V 128                    /* 2^7 possible column vectors */

/* Power of 3 for state encoding */
static const int pow3[ROWS] = {1, 3, 9, 27, 81, 243, 729};

/* Table: digits[row] of a state index (base‑3 representation) */
static uint8_t state_digits[STATE_COUNT][ROWS];

/* Column weights modulo 7 for rows 0..6 */
static const int col_weight[ROWS] = {1, 4, 2, 1, 4, 2, 1};

/* Allowed column vectors and their count */
static uint8_t allowed_mask[MAX_V];
static int countV = 0;

/* Transition tables: trans[weight_type][state][vector_index] -> destination_state
 *   weight_type 0 = column weight = 1 (add), 1 = column weight = 2 (subtract)   */
static uint16_t trans[2][STATE_COUNT][MAX_V];

/* initialization flag */
static int initialized = 0;

/* ---- helper to compute state digits once ---- */
static void init_state_digits(void)
{
    for (int s = 0; s < STATE_COUNT; ++s) {
        int tmp = s;
        for (int r = 0; r < ROWS; ++r) {
            state_digits[s][r] = tmp % 3;
            tmp /= 3;
        }
    }
}

/* ---- initialization of data structures ---- */
static void init(void)
{
    int p;
    /* precompute state digits */
    init_state_digits();

    /* find all column vectors that satisfy the column mod‑7 condition */
    for (int mask = 0; mask < (1 << ROWS); ++mask) {
        int sum = 0;
        for (int r = 0; r < ROWS; ++r) {
            if (mask & (1 << r))
                sum = (sum + col_weight[r]) % 7;
        }
        if (sum == 0) {
            allowed_mask[countV++] = (uint8_t)mask;
        }
    }

    /* build transition tables */
    for (int wt = 0; wt < 2; ++wt) {          /* 0: weight 1, 1: weight 2 */
        for (int s = 0; s < STATE_COUNT; ++s) {
            const uint8_t *digits = state_digits[s];
            for (int v = 0; v < countV; ++v) {
                int mask = allowed_mask[v];
                int dest = 0;
                for (int r = 0; r < ROWS; ++r) {
                    int bit = (mask >> r) & 1;
                    int add;
                    if (wt == 0)           /* weight 1: add 0 or 1 */
                        add = (digits[r] + bit) % 3;
                    else                   /* weight 2: add 0 or 2 */
                        add = (digits[r] + ((bit * 2) % 3)) % 3;
                    dest += add * pow3[r];
                }
                trans[wt][s][v] = (uint16_t)dest;
            }
        }
    }

    initialized = 1;
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    if (!initialized)
        init();

    /* number of columns = n+1 */
    size_t cols = (size_t)n + 1;

    /* DP arrays */
    static seq_t cur[STATE_COUNT];
    static seq_t nxt[STATE_COUNT];

    /* initialize DP */
    for (int i = 0; i < STATE_COUNT; ++i)
        cur[i] = 0;
    cur[0] = (seq_t)1;

    /* iterate through columns */
    for (size_t j = 0; j < cols; ++j) {
        /* determine column weight (1 or 2) for this position */
        int wt;
        if ((n & 1U) == 0U)          /* n even: pattern 1,2,1,2,... */
            wt = (j & 1U) == 0U ? 0 : 1;
        else                         /* n odd : pattern 2,1,2,1,... */
            wt = (j & 1U) == 1U ? 0 : 1;   /* 0 = weight 1, 1 = weight 2 */

        /* zero next array */
        for (int i = 0; i < STATE_COUNT; ++i)
            nxt[i] = 0;

        /* transition */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = cur[s];
            if (val == (seq_t)0)
                continue;
            /* add contributions of all allowed column vectors */
            uint16_t *row_trans = (uint16_t *)trans[wt][s];
            for (int v = 0; v < countV; ++v) {
                int dest = row_trans[v];
                nxt[dest] += val;
            }
        }

        /* swap arrays */
        for (int i = 0; i < STATE_COUNT; ++i)
            cur[i] = nxt[i];
    }

    return cur[0];
}
