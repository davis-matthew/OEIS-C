/*
 * A297461  Number of 5Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0 or 2 neighboring 1s.
 * https://oeis.org/A297461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5xn 0..1 matrices such that every 1 has 0 or 2
 *            of its horizontally, diagonally or antidiagonally adjacent cells
 *            also 1. Computed by dynamic programming on the column‑by‑column
 *            state
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

enum { ROWS = 5, STATES = 1 << 10, /* 4^5 states */ MAX_TRANS = 256 };

/* Transition tables, built once */
static int trans_to[STATES][MAX_TRANS];
static int trans_cnt[STATES];

/* Jumps immediately on first use */
static void init_tables(void) {
    static int ready = 0;
    if (ready) return;
    ready = 1;

    for (int s = 0; s < STATES; ++s) {
        /* Decode previous column bits and requirement codes */
        int prev_code[ROWS];
        int prev_bits[ROWS];
        for (int r = 0; r < ROWS; ++r) {
            int c = (s >> (r * 2)) & 3;
            prev_code[r] = c;                    /* 0..3 : 0=none, 1..3=1 with rem 0..2 */
            prev_bits[r] = (c != 0) ? 1 : 0;     /* previous column cell contains 1 */
        }

        int trans_index = 0;

        /* Enumerate all possible patterns for the new column */
        for (int p = 0; p < (1 << ROWS); ++p) {
            /* 1. All previous cells with a requirement must receive that many 1s
               from the new column (their left/right/diag neighbours). */
            int ok = 1;
            for (int r = 0; r < ROWS && ok; ++r) {
                int need = prev_code[r] - 1;       /* remaining from right (=needed from new column) */
                if (prev_code[r] == 0) continue;   /* no 1 in this cell → no constraint */
                int cnt = 0;
                if (p & (1 << (r - 1))) cnt++;
                if (p & (1 <<  r      )) cnt++;
                if (p & (1 << (r + 1))) cnt++;
                if (cnt != need) ok = 0;
            }
            if (!ok) continue;

            /* 2. Pre‑compute left‑neighbor counts for each new cell. */
            int left_cnt[ROWS];
            for (int r = 0; r < ROWS; ++r) {
                int sum = prev_bits[r];
                if (r > 0)    sum += prev_bits[r - 1];
                if (r < ROWS-1) sum += prev_bits[r + 1];
                left_cnt[r] = sum;
            }

            /* 3. Build flexible positions (new 1 with left_cnt==0). */
            int flex_pos[ROWS];
            int flex_idx[ROWS];
            int flex_cnt = 0;
            for (int r = 0; r < ROWS; ++r) {
                flex_idx[r] = -1;
            }
            for (int r = 0; r < ROWS; ++r) {
                if ((p >> r) & 1) {              /* new cell is 1 */
                    if (left_cnt[r] > 2) {        /* impossible */
                        ok = 0; break;
                    }
                    if (left_cnt[r] == 0) {
                        flex_pos[flex_cnt++] = r;
                        flex_idx[r] = flex_cnt - 1;
                    }
                }
            }
            if (!ok) continue;

            /* 4. Enumerate all combinations of remaining requirements for flexible cells. */
            int combos = 1 << flex_cnt;
            for (int mask = 0; mask < combos; ++mask) {
                int new_state = 0;
                for (int r = 0; r < ROWS; ++r) {
                    int code = 0;                /* default: cell 0 → state 0 */

                    if ((p >> r) & 1) {          /* new cell is 1 */
                        int lc = left_cnt[r];
                        if (lc == 0) {
                            int f = flex_idx[r];
                            int bit = (mask >> f) & 1;
                            code = (bit == 0) ? 1 : 3; /* remaining 0 or 2 */
                        } else if (lc == 1) {
                            code = 2;             /* remaining 1 */
                        } else { /* lc==2 */
                            code = 1;             /* remaining 0 */
                        }
                    }
                    new_state |= code << (r * 2);
                }
                trans_to[s][trans_index++] = new_state;
            }
        }
        trans_cnt[s] = trans_index;
    }
}

/* --- Main sequence function ------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1 */

    init_tables();                  /* build transition tables once */

    static seq_t dp_cur[STATES];
    static seq_t dp_next[STATES];
    seq_t *cur = dp_cur;
    seq_t *next = dp_next;

    /* initial state: virtual column before first real column (all zeros) */
    for (int i = 0; i < STATES; ++i) cur[i] = 0;
    cur[0] = 1;                     /* state 0 has all zeros */

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < STATES; ++i) next[i] = 0; /* clear next */

        for (int s = 0; s < STATES; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            int cnt = trans_cnt[s];
            for (int j = 0; j < cnt; ++j) {
                int t = trans_to[s][j];
                next[t] += val;
            }
        }
        /* swap pointers */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* sum all states that are "final": no remaining requirement (codes 0 or 1) */
    seq_t result = 0;
    for (int s = 0; s < STATES; ++s) {
        int ok = 1;
        for (int r = 0; r < ROWS; ++r) {
            if (((s >> (r * 2)) & 3) >= 2) { ok = 0; break; }
        }
        if (ok) result += cur[s];
    }
    return result;
}
