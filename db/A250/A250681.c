/*
 * A250681  Number of (5+1)X(n+1) 0..3 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 13, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of 6×(n+1) arrays with entries 0..3 satisfying
 *  the nondecreasing “row‑sum” and “column‑difference” constraints.
 *  Computed by dynamic programming over column states. */

#include <stdint.h>
#include <stdbool.h>

#define MAX_STATES 4096            /* 4^6 possible column vectors      */
#define MAX_DEGREE 256             /* safety bound on outgoing edges  */

static uint8_t  col_val[MAX_STATES][6];
static uint8_t  col_diff[MAX_STATES][5];
static uint16_t adj[MAX_STATES][MAX_DEGREE];
static uint16_t adj_len[MAX_STATES];
static bool     graph_built = false;

/* ----------------------------------------------------------------------
 * Build the transition graph once.
 * ---------------------------------------------------------------------- */
static void build_graph(void)
{
    if (graph_built) return;

    /* precompute vectors and their row‑difference patterns */
    for (uint32_t id = 0; id < MAX_STATES; ++id) {
        uint32_t tmp = id;
        for (int i = 0; i < 6; ++i) {
            col_val[id][i] = tmp & 3;   /* two bits per entry, 0..3 */
            tmp >>= 2;
        }
        for (int i = 1; i < 6; ++i) {
            uint8_t a = col_val[id][i];
            uint8_t b = col_val[id][i-1];
            col_diff[id][i-1] = (a > b) ? a - b : b - a;
        }
    }

    /* build adjacency: for each previous column p, list all admissible
       next columns q.  A transition is allowed iff
          1) x(p,i)+x(q,i) is nondecreasing in i
          2) |x(q,i)-x(q,i-1)| >= |x(p,i)-x(p,i-1)|  for all i>0
    */
    for (uint32_t p = 0; p < MAX_STATES; ++p) {
        adj_len[p] = 0;
        for (uint32_t q = 0; q < MAX_STATES; ++q) {
            /* condition 1: row‑sum monotonicity */
            int s = col_val[p][0] + col_val[q][0];
            bool ok = true;
            for (int i = 1; i < 6 && ok; ++i) {
                int ns = col_val[p][i] + col_val[q][i];
                if (s > ns) ok = false;   /* violation */
                s = ns;
            }
            if (!ok) continue;

            /* condition 2: column‑difference monotonicity */
            for (int i = 1; i < 6; ++i) {
                if (col_diff[q][i-1] < col_diff[p][i-1]) {
                    ok = false; break;
                }
            }
            if (!ok) continue;

            if (adj_len[p] < MAX_DEGREE)
                adj[p][adj_len[p]++] = (uint16_t)q;
        }
    }

    graph_built = true;
}

/* ----------------------------------------------------------------------
 * Main sequence function.
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n >= 1 by specification; we count transitions for n steps. */
    build_graph();

    static seq_t dp[MAX_STATES];
    static seq_t tmp[MAX_STATES];
    seq_t *dp_ptr  = dp;
    seq_t *tmp_ptr = tmp;

    /* initialise with column 0 any vector */
    for (int i = 0; i < MAX_STATES; ++i) dp_ptr[i] = 1;

    /* iterate over columns */
    for (idx_t step = 1; step <= n; ++step) {
        /* clear temporary vector */
        for (int i = 0; i < MAX_STATES; ++i) tmp_ptr[i] = 0;

        for (int p = 0; p < MAX_STATES; ++p) {
            seq_t val = dp_ptr[p];
            if (val == 0) continue;          /* skip empty predecessors */
            uint16_t *e = adj[p];
            uint16_t *e_end = e + adj_len[p];
            while (e != e_end) {
                tmp_ptr[*e] += val;
                ++e;
            }
        }
        /* swap dp and tmp pointers for next iteration */
        seq_t *swap = dp_ptr; dp_ptr = tmp_ptr; tmp_ptr = swap;
    }

    /* sum over all final column vectors */
    seq_t result = 0;
    for (int i = 0; i < MAX_STATES; ++i) result += dp_ptr[i];
    return result;
}
