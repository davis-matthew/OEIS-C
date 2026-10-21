/*
 * A250989  Number of (n+1)X(3+1) 0..2 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A250989 (OEIS) – Count of (n+1)×4 arrays over {0,1,2} with
   * min(x(i,j),x(i,j-1)) nondecreasing in the i‑direction
   * and |x(i,j)-x(i-1,j)| nondecreasing across j in the j‑direction.
   *
   * The sequence is computed by dynamic programming.  The state of each
   * row of the array is a 4‑tuple of digits 0..2 (81 possibilities).
   * For each pair of consecutive rows a transition is allowed if the
   * above conditions hold.  Transition uses a 81×81 boolean matrix.
   * Given n, the number of arrays of size (n+1)×4 equals the number of
   * paths of length n in this directed graph, i.e. the sum of the
   * entries of the dp vector after n transitions.  This algorithm
   * runs in O(n·81²) time and uses only constant additional space.
   * It works for any seq_t and idx_t supplied by the build; see
   * the requirements for type definitions.  It does not use any
   * external helpers.
*/

#include <stdint.h>  /* for uint8_t */

/* Transition matrix and finite‑state precomputation */
static uint8_t trans[81][81];
static int state[81][4];
static int initialized = 0;

/* Build transition matrix once */
static void init_matrix(void)
{
    if (initialized) return;
    /* Encode each of the 81 possible rows. */
    for (int s = 0; s < 81; ++s) {
        int x = s;
        for (int j = 0; j < 4; ++j) {
            state[s][j] = x % 3;
            x /= 3;
        }
    }
    /* Construct transitions */
    for (int p = 0; p < 81; ++p) {
        for (int c = 0; c < 81; ++c) {
            /* Condition 1: min(x(i,j),x(i,j-1)) nondecreasing. */
            int ok = 1;
            for (int j = 1; j < 4 && ok; ++j) {
                int minC = state[c][j] < state[c][j - 1] ? state[c][j] : state[c][j - 1];
                int minP = state[p][j] < state[p][j - 1] ? state[p][j] : state[p][j - 1];
                if (minC < minP) ok = 0;
            }
            /* Condition 2: differences |x(i,j)-x(i-1,j)| nondecreasing across j. */
            if (ok) {
                int d0 = state[c][0] >= state[p][0] ? state[c][0] - state[p][0] : state[p][0] - state[c][0];
                int d1 = state[c][1] >= state[p][1] ? state[c][1] - state[p][1] : state[p][1] - state[c][1];
                int d2 = state[c][2] >= state[p][2] ? state[c][2] - state[p][2] : state[p][2] - state[c][2];
                int d3 = state[c][3] >= state[p][3] ? state[c][3] - state[p][3] : state[p][3] - state[c][3];
                if (d0 > d1 || d1 > d2 || d2 > d3) ok = 0;
            }
            trans[p][c] = ok ? 1 : 0;
        }
    }
    initialized = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used but harmless */

    init_matrix();

    /* dp[s] – number of arrays ending in row state s after current number of transitions */
    seq_t dp[81];
    seq_t next[81];
    for (int s = 0; s < 81; ++s) dp[s] = (seq_t)1;   /* first row any of 81 states */

    /* Perform n transitions to reach (n+1) rows */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero next vector */
        for (int s = 0; s < 81; ++s) next[s] = (seq_t)0;
        /* Transition */
        for (int p = 0; p < 81; ++p) {
            if (dp[p] == (seq_t)0) continue;
            for (int c = 0; c < 81; ++c) {
                if (trans[p][c]) {
                    next[c] += dp[p];
                }
            }
        }
        /* Copy back */
        for (int s = 0; s < 81; ++s) dp[s] = next[s];
    }

    /* Sum all final states to obtain a(n) */
    seq_t total = (seq_t)0;
    for (int s = 0; s < 81; ++s) total += dp[s];

    return total;
}
