/*
 * A250996  Number of (2+1)X(n+1) 0..2 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 33, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of 3×(n+1) 0..2 arrays with
 * nondecreasing min(x(i,j),x(i,j-1)) in the i direction
 * and nondecreasing |x(i,j)-x(i-1,j)| in the j direction.
 *
 * Computation is done by dynamic programming over column
 * transitions.  There are 27 possible column states
 * (each of the 3 entries ∈ {0,1,2}).  A transition from state
 * p to state c is allowed iff
 *   – min(p[i],c[i]) is nondecreasing in i (i=0,1)
 *   – |c[1]-c[0]| ≥ |p[1]-p[0]| and |c[2]-c[1]| ≥ |p[2]-p[1]|.
 *
 * Starting from a single column (any of the 27 states), we
 * perform n transitions to obtain n+1 columns; the sum of counts
 * over all states after n transitions is a(n).  No dynamic
 * allocation is needed and the routine works for any integer
 * width for seq_t, including arbitrary‑precision classes.
 */

#include <stddef.h>
#include <stdbool.h>

/* Transition matrix – computed on first use */
static bool transition_matrix[27][27];
static bool transition_computed = false;

static void compute_transition_matrix(void)
{
    for (int pi = 0; pi < 27; ++pi) {
        int p0 = pi / 9;
        int p1 = (pi / 3) % 3;
        int p2 = pi % 3;
        for (int ci = 0; ci < 27; ++ci) {
            int c0 = ci / 9;
            int c1 = (ci / 3) % 3;
            int c2 = ci % 3;

            /* min condition: min(p[i],c[i]) nondecreasing in i (i=0,1) */
            int m0 = (p0 < c0) ? p0 : c0;
            int m1 = (p1 < c1) ? p1 : c1;
            int m2 = (p2 < c2) ? p2 : c2;
            if (!(m0 <= m1 && m1 <= m2)) {
                transition_matrix[pi][ci] = false;
                continue;
            }

            /* difference condition: differences nondecreasing */
            int dOld1 = (p1 > p0) ? (p1 - p0) : (p0 - p1);
            int dOld2 = (p2 > p1) ? (p2 - p1) : (p1 - p2);
            int dNew1 = (c1 > c0) ? (c1 - c0) : (c0 - c1);
            int dNew2 = (c2 > c1) ? (c2 - c1) : (c1 - c2);
            if (dNew1 < dOld1 || dNew2 < dOld2) {
                transition_matrix[pi][ci] = false;
                continue;
            }

            transition_matrix[pi][ci] = true;
        }
    }
    transition_computed = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, as per offset */

    /* Ensure transition matrix has been built */
    if (!transition_computed) compute_transition_matrix();

    /* DP arrays: dp_cur holds counts for current column state,
     * dp_next holds counts for next column state. */
    seq_t dp_cur[27];
    seq_t dp_next[27];

    /* Initial column: any of the 27 states is possible */
    for (int i = 0; i < 27; ++i) dp_cur[i] = (seq_t)1;

    /* Perform n transitions → n+1 columns */
    for (idx_t step = 0; step < n; ++step) {
        /* zero next array */
        for (int i = 0; i < 27; ++i) dp_next[i] = (seq_t)0;

        /* transition */
        for (int pi = 0; pi < 27; ++pi) {
            seq_t val = dp_cur[pi];
            if (!val) continue;
            for (int ci = 0; ci < 27; ++ci) {
                if (transition_matrix[pi][ci]) {
                    dp_next[ci] += val;
                }
            }
        }

        /* copy next to current for next iteration */
        for (int i = 0; i < 27; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum over all column states to obtain a(n) */
    seq_t sum = (seq_t)0;
    for (int i = 0; i < 27; ++i) sum += dp_cur[i];

    return sum;
}
