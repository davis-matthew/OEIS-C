/*
 * A277654  Number of nX3 0..2 arrays with every element equal to some element
 *          at offset (-1,0) (-1,1) (0,-1) (0,1) or (1,0) both plus 1 mod 3
 *          and minus 1 mod 3, with new values introduced in order 0..2.
 * https://oeis.org/A277654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×3 arrays over {0,1,2} such that * for every element
 *            there is a neighbour at offsets * (-1,0), (-1,1), (0,-1), (0,1),
 *            (1,0) equal to the * element +1 mod 3 and another equal to the
 *            element –1 mod 3, * and new values are introduced in the
 *            scanning order 0,1,2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define ROWS 27          /* 3^3 possible rows of length 3 */
#define BITS 6           /* 3 columns × 2 conditions ( +1, –1 ) */
#define STATE_STRIDE (ROWS * (1 << BITS) * 2) /* rows × masks × seen flag */

static int row_colors[ROWS][3];
static int row_no_up_mask[ROWS];            /* missing mask when there is no up row */
static int has_one_row[ROWS];               /* row contains colour 1 */
static int has_two_row[ROWS];               /* row contains colour 2 */
static int allowed_unseen[ROWS];            /* can be added when no 1 seen yet */

/* Transition tables
 * ready_mask[r][q] – 6‑bit bitmask of conditions in row r that
 *                     can be satisfied by the down neighbour q[i].
 * next_mask[r][q] – 6‑bit missing mask for row q after considering
 *                   up neighbors from r and left/right neighbours in q. */
static int ready_mask[ROWS][ROWS];
static int next_mask[ROWS][ROWS];

/* For efficient DP – adjacency list for each state
 * state_id = ((r * (1 << BITS)) + mask) * 2 + seen_flag
 * 0 ≤ state_id < MAX_STATES  (≈3456)
 */
#define MAX_STATES (ROWS * (1 << BITS) * 2)

static int trans_next[MAX_STATES][27];
static int trans_cnt[MAX_STATES];

/* Map for initial states after the first row:
 * init_state[r] = state_id or -1 if row r is not a valid first row. */
static int init_state[ROWS];

/* Pre‑computation routine – runs once. */
static void precompute(void) {
    static int done = 0;
    if (done) return;
    done = 1;

    /* Enumerate all 27 possible rows. */
    int idx = 0;
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c) {
                row_colors[idx][0] = a;
                row_colors[idx][1] = b;
                row_colors[idx][2] = c;
                ++idx;
            }

    /* Pre‑compute data for each row. */
    for (int r = 0; r < ROWS; ++r) {
        int mask = 0;
        int first_one = 3, first_two = 3;
        has_one_row[r] = 0;
        has_two_row[r] = 0;
        for (int i = 0; i < 3; ++i) {
            int col = row_colors[r][i];
            if (col == 1) {
                has_one_row[r] = 1;
                if (first_one == 3) first_one = i;
            }
            if (col == 2) {
                has_two_row[r] = 1;
                if (first_two == 3) first_two = i;
            }
        }
        if (has_two_row[r] && (!has_one_row[r] || first_two < first_one))
            allowed_unseen[r] = 0;
        else
            allowed_unseen[r] = 1;

        /* Missing mask when there is no up row – only left/right neighbours. */
        int missing = 63; /* all 6 bits set */
        for (int i = 0; i < 3; ++i) {
            int col = row_colors[r][i];
            if (i > 0) {
                int neighbour = row_colors[r][i-1];
                if (neighbour == (col+1)%3) missing &= ~(1<<i);
                if (neighbour == (col+2)%3) missing &= ~(1<<(i+3));
            }
            if (i < 2) {
                int neighbour = row_colors[r][i+1];
                if (neighbour == (col+1)%3) missing &= ~(1<<i);
                if (neighbour == (col+2)%3) missing &= ~(1<<(i+3));
            }
        }
        row_no_up_mask[r] = missing;
    }

    /* Pre‑compute transition data between rows. */
    for (int r = 0; r < ROWS; ++r) {
        for (int q = 0; q < ROWS; ++q) {
            /* ready_mask – which missing conditions of r can be satisfied
               by the down neighbour from q (same column). */
            int rm = 0;
            for (int i = 0; i < 3; ++i) {
                int rcol = row_colors[r][i];
                int qcol = row_colors[q][i];
                if (qcol == (rcol+1)%3) rm |= 1<<i;
                if (qcol == (rcol+2)%3) rm |= 1<<(i+3);
            }
            ready_mask[r][q] = rm;

            /* next_mask – missing mask for q after using up neighbours
               from r and left/right neighbours in q. */
            int nm = 63;
            for (int i = 0; i < 3; ++i) {
                int qcol = row_colors[q][i];
                /* left neighbour */
                if (i > 0) {
                    int n = row_colors[q][i-1];
                    if (n == (qcol+1)%3) nm &= ~(1<<i);
                    if (n == (qcol+2)%3) nm &= ~(1<<(i+3));
                }
                /* right neighbour */
                if (i < 2) {
                    int n = row_colors[q][i+1];
                    if (n == (qcol+1)%3) nm &= ~(1<<i);
                    if (n == (qcol+2)%3) nm &= ~(1<<(i+3));
                }
                /* up neighbour from r */
                int n = row_colors[r][i];
                if (n == (qcol+1)%3) nm &= ~(1<<i);
                if (n == (qcol+2)%3) nm &= ~(1<<(i+3));
                /* up‑right neighbour */
                if (i+1 < 3) {
                    n = row_colors[r][i+1];
                    if (n == (qcol+1)%3) nm &= ~(1<<i);
                    if (n == (qcol+2)%3) nm &= ~(1<<(i+3));
                }
            }
            next_mask[r][q] = nm;
        }
    }

    /* Build transition adjacency list. */
    for (int r = 0; r < ROWS; ++r) {
        for (int m = 0; m < (1 << BITS); ++m) {
            for (int seen = 0; seen < 2; ++seen) {
                int state_id = ((r * (1 << BITS)) + m) * 2 + seen;
                trans_cnt[state_id] = 0;
                for (int q = 0; q < ROWS; ++q) {
                    /* Respect the 'new values introduced in order 0..2' rule. */
                    if (!seen && !allowed_unseen[q]) continue;

                    /* All missing conditions of the previous row must
                       be satisfied by the down neighbour from q. */
                    if ( (m & ~(ready_mask[r][q])) != 0 ) continue;

                    int new_mask = next_mask[r][q];
                    int new_seen = seen | has_one_row[q];
                    int next_state = ((q * (1 << BITS)) + new_mask) * 2 + new_seen;
                    trans_next[state_id][trans_cnt[state_id]++] = next_state;
                }
            }
        }
    }

    /* Build initial states after adding the first row. */
    for (int r = 0; r < ROWS; ++r) {
        init_state[r] = -1;
        if (row_colors[r][0] != 0) continue;          /* first cell must be 0 */
        if (!allowed_unseen[r]) continue;             /* first row must also respect order */
        int m = row_no_up_mask[r];
        int seen = has_one_row[r];
        init_state[r] = ((r * (1 << BITS)) + m) * 2 + seen;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    precompute();

    static seq_t cur[MAX_STATES];
    static seq_t nxt[MAX_STATES];
    seq_t *curr = cur;
    seq_t *next = nxt;

    /* initialise */
    for (int i = 0; i < MAX_STATES; ++i) curr[i] = 0;
    for (int r = 0; r < ROWS; ++r) {
        if (init_state[r] >= 0) {
            curr[init_state[r]] = 1;
        }
    }

    /* if only one row, answer is sum over states with no missing conditions */
    if (n == 1) {
        seq_t result = 0;
        for (int s = 0; s < MAX_STATES; ++s) {
            int mask = (s >> 1) & 0x3F;
            if (mask == 0) result += curr[s];
        }
        return result;
    }

    /* add remaining rows */
    for (idx_t step = 1; step < n; ++step) {
        /* zero next array */
        for (int i = 0; i < MAX_STATES; ++i) next[i] = 0;

        for (int s = 0; s < MAX_STATES; ++s) {
            seq_t val = curr[s];
            if (val == 0) continue;
            int tcnt = trans_cnt[s];
            for (int t = 0; t < tcnt; ++t) {
                int ns = trans_next[s][t];
                next[ns] += val;
            }
        }
        /* swap */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    /* final answer – last row must have all conditions satisfied */
    seq_t answer = 0;
    for (int s = 0; s < MAX_STATES; ++s) {
        int mask = (s >> 1) & 0x3F;
        if (mask == 0) answer += curr[s];
    }
    return answer;
}
