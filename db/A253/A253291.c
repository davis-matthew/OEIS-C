/*
 * A253291  Number of (n+1)X(3+1) 0..2 arrays with every 2X2 subblock sum
 *          differing from its neighbors horizontally, vertically, diagonally
 *          and antidiagonally.
 * https://oeis.org/A253291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: DP over 4‑digit rows in base 3.
   States are ordered pairs (prevprev,prev) of 4‑digit rows.
   Transition (prevprev,prev) → (prev,cur) is allowed iff
   1) the pair (prev,cur) is horizontally unique:
          sum(prev,cur)[0] != sum(prev,cur)[1] &&
          sum(prev,cur)[1] != sum(prev,cur)[2]   (sums of overlapping 2×2 blocks).
   2) for each column k = 0..2:
          sum(prev,cur)[k] != sum(prevprev,prev)[k]            (vertical)
          if k>0  sum(prev,cur)[k] != sum(prevprev,prev)[k-1]  (diagonal)
          if k<2  sum(prev,cur)[k] != sum(prevprev,prev)[k+1]  (anti‑diagonal)
   The sequence a(n) is the sum of counts of all states after n − 1
   transitions, i.e. after forming an (n+1) × 4 array. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Constants */
#define ROWS         81   /* 3^4 distinct 4‑digit rows (base 3) */
#define STATE_COUNT  (81*81)  /* (prevprev,prev) states: 6561 */
#define MAX_DEG      81     /* at most 81 successors per state */

static bool tables_built = false;

/* row pattern -> 4 digits (base‑3) */
static uint8_t row_digits[ROWS][4];
/* sums of the 3 two‑by‑two blocks for each ordered pair of rows */
static uint8_t pair_sums[STATE_COUNT][3];
/* horizontal uniqueness flag for each pair of rows (1 = ok, 0 = bad) */
static uint8_t pair_ok[STATE_COUNT];
/* successors of each state: list of destination state indices */
static uint16_t succ[STATE_COUNT][MAX_DEG];
static uint8_t    succ_cnt[STATE_COUNT];

/* Build all tables once */
static void build_tables(void)
{
    /* 1. Pre‑compute 4‑digit representation for each row pattern */
    for (unsigned int idx = 0; idx < ROWS; ++idx) {
        unsigned int t = idx;
        for (int c = 3; c >= 0; --c) {
            row_digits[idx][c] = t % 3;
            t /= 3;
        }
    }

    /* 2. Compute sums and horizontal_ok for every ordered pair of rows */
    for (unsigned int i = 0; i < ROWS; ++i) {
        for (unsigned int j = 0; j < ROWS; ++j) {
            unsigned int sidx = i * ROWS + j;   /* state index (prevprev,prev) */
            uint8_t s[3];
            for (int k = 0; k < 3; ++k) {
                s[k] = row_digits[i][k] + row_digits[i][k + 1] +
                       row_digits[j][k] + row_digits[j][k + 1];
            }
            pair_sums[sidx][0] = s[0];
            pair_sums[sidx][1] = s[1];
            pair_sums[sidx][2] = s[2];
            /* horizontal uniqueness: adjacent sums must differ */
            pair_ok[sidx] = (s[0] != s[1] && s[1] != s[2]) ? 1 : 0;
        }
    }

    /* 3. Compute successors for every state */
    for (unsigned int i = 0; i < ROWS; ++i) {
        for (unsigned int j = 0; j < ROWS; ++j) {
            unsigned int sidx = i * ROWS + j;          /* state (prevprev,prev) */
            unsigned int cnt = 0;
            for (unsigned int cur = 0; cur < ROWS; ++cur) {
                unsigned int didx = j * ROWS + cur;    /* destination (prev,cur) */
                if (!pair_ok[didx]) continue;         /* horizontal constraint */

                const uint8_t *sPrev = pair_sums[sidx];
                const uint8_t *sCur  = pair_sums[didx];
                bool bad = false;
                /* vertical and diagonal constraints for the 3 sums */
                for (int k = 0; k < 3; ++k) {
                    if (sCur[k] == sPrev[k]) { bad = true; break; }
                    if (k > 0   && sCur[k] == sPrev[k - 1]) { bad = true; break; }
                    if (k < 2   && sCur[k] == sPrev[k + 1]) { bad = true; break; }
                }
                if (!bad) {
                    succ[sidx][cnt++] = (uint16_t)didx;
                }
            }
            succ_cnt[sidx] = (uint8_t)cnt;
        }
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (!tables_built) {
        build_tables();
        tables_built = true;
    }

    static seq_t dp_a[STATE_COUNT];
    static seq_t dp_b[STATE_COUNT];
    seq_t *dp   = dp_a;
    seq_t *next = dp_b;

    /* initial state: all pairs of rows with horizontal_ok true */
    for (unsigned int i = 0; i < STATE_COUNT; ++i) {
        dp[i] = pair_ok[i] ? (seq_t)1 : (seq_t)0;
    }

    /* number of additional rows to add */
    size_t steps = (size_t)(n - 1);   /* n ≥ 1, so steps ≥ 0 */
    for (size_t step = 0; step < steps; ++step) {
        /* zero the next array */
        for (unsigned int i = 0; i < STATE_COUNT; ++i) {
            next[i] = (seq_t)0;
        }

        /* transition */
        for (unsigned int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;
            const uint16_t *dest = succ[s];
            uint8_t cnt = succ_cnt[s];
            for (uint8_t t = 0; t < cnt; ++t) {
                next[dest[t]] += val;
            }
        }

        /* swap dp and next for the next iteration */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    /* sum all states to obtain the answer */
    seq_t result = (seq_t)0;
    for (unsigned int i = 0; i < STATE_COUNT; ++i) {
        result += dp[i];
    }
    return result;
}
