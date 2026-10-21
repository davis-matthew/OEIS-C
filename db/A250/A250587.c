/*
 * A250587  Number of (n+1)X(3+1) 0..2 arrays with nondecreasing
 *          max(x(i,j),x(i,j-1)) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 12, u64 n <= 29, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over rows. For each row a state
   is a sequence of 4 cells (values 0,1,2).  Transitions between
   states are pre‑computed once.  a(n) counts (n+1)×4 arrays that
   satisfy the two monotonicity conditions given in the OEIS entry. */

#include <stdint.h>

static uint8_t state_digits[81][4];
static uint8_t next_of[81][81];
static uint8_t next_count[81];
static int initialized = 0;

static void init_states(void)
{
    if (initialized) return;
    /* enumerate all possible rows */
    for (int s = 0; s < 81; ++s) {
        int t = s;
        for (int j = 0; j < 4; ++j) {
            state_digits[s][j] = t % 3;
            t /= 3;
        }
    }
    /* compute allowed transitions */
    for (int prev = 0; prev < 81; ++prev) {
        for (int curr = 0; curr < 81; ++curr) {
            int ok = 1;
            /* nondecreasing max(x(i,j),x(i,j-1)) in the i direction */
            for (int j = 1; j < 4; ++j) {
                uint8_t prev_max = state_digits[prev][j] > state_digits[prev][j-1]
                                  ? state_digits[prev][j] : state_digits[prev][j-1];
                uint8_t curr_max = state_digits[curr][j] > state_digits[curr][j-1]
                                  ? state_digits[curr][j] : state_digits[curr][j-1];
                if (curr_max < prev_max) { ok = 0; break; }
            }
            if (!ok) continue;
            /* nondecreasing min(x(i,j),x(i-1,j)) in the j direction */
            for (int j = 1; j < 4; ++j) {
                uint8_t m_curr = state_digits[curr][j] < state_digits[prev][j]
                               ? state_digits[curr][j] : state_digits[prev][j];
                uint8_t m_prev = state_digits[curr][j-1] < state_digits[prev][j-1]
                               ? state_digits[curr][j-1] : state_digits[prev][j-1];
                if (m_curr < m_prev) { ok = 0; break; }
            }
            if (ok) {
                next_of[prev][next_count[prev]++] = curr;
            }
        }
    }
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 0 */
    init_states();

    seq_t cur[81], next[81];
    for (int s = 0; s < 81; ++s) cur[s] = 1; /* first row arbitrary */

    for (idx_t t = 1; t <= n; ++t) { /* iterate over the remaining rows */
        for (int s = 0; s < 81; ++s) next[s] = 0;
        for (int prev = 0; prev < 81; ++prev) {
            seq_t val = cur[prev];
            if (!val) continue;
            for (int k = 0; k < next_count[prev]; ++k) {
                int curr = next_of[prev][k];
                next[curr] += val;
            }
        }
        for (int s = 0; s < 81; ++s) cur[s] = next[s];
    }

    seq_t total = 0;
    for (int s = 0; s < 81; ++s) total += cur[s];
    return total;
}
