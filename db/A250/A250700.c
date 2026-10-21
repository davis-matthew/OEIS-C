/*
 * A250700  Number of (n+1)X(1+1) 0..3 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 35, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) counts arrays of size (n+1) × 2 with entries 0..3 
 *  satisfying the constraints:
 *    • For each row i the sum s_i = x(i,0)+x(i,1) is nondecreasing in i.
 *    • For each adjacent pair of rows (i‑1,i) the values
 *      min(x(i‑1,0),x(i,0)) and min(x(i‑1,1),x(i,1)) are nondecreasing
 *      as j goes from 0 to 1.
 *
 *  The number a(n) is obtained by dynamic programming over the 16
 *  possible row states (a,b).  Let C be the 16×16 adjacency matrix
 *  with C[p][q]=1 when row q can follow row p respecting the above
 *  constraints.  If v_i is a vector of counts of ways to end at each
 *  state after i steps (i rows of the array), then v_{i+1}=v_i·C.
 *  The value a(n) for a(n) with n≥1 is the sum of the entries of
 *  v_n, where v_0 is the vector of all 1’s (any first row is allowed).
 *
 *  This algorithm runs in O(n·256) time and O(256) memory per n, memoising
 *  results for all n requested so far.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static bool adj_init = false;
static bool adj[16][16];

/* initialise the 16×16 adjacency matrix once */
static void init_adj(void)
{
    if (adj_init) return;
    for (int ip = 0; ip < 16; ++ip) {
        int a_prev = ip >> 2;           /* a_prev = ip / 4  */
        int b_prev = ip & 3;            /* b_prev = ip % 4  */
        int s_prev = a_prev + b_prev;
        for (int iq = 0; iq < 16; ++iq) {
            int a_curr = iq >> 2;
            int b_curr = iq & 3;
            int s_curr = a_curr + b_curr;
            bool cond1 = s_prev <= s_curr;
            int min0 = a_prev < a_curr ? a_prev : a_curr;
            int min1 = b_prev < b_curr ? b_prev : b_curr;
            bool cond2 = min0 <= min1;
            adj[ip][iq] = cond1 && cond2;
        }
    }
    adj_init = true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache[0] will be a(1) */
    static idx_t cache_len = 0;          /* number of terms cached          */
    static seq_t cur[16];                /* counts after current number of steps */
    static seq_t next[16];
    static idx_t cur_steps = 0;           /* steps performed (cur corresponds to step cur_steps) */

    /* initialise on first call */
    if (cache_len == 0) {
        init_adj();
        for (int i = 0; i < 16; ++i) cur[i] = (seq_t)1; /* v_0 : one way for each first row */
        cur_steps = 0;
    }

    /* expand cache if needed */
    if (n > cache_len) {
        /* enlarge cache array to hold up to n terms */
        cache = (seq_t *)realloc(cache, ((size_t)n) * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);  /* allocation failed */

        /* compute additional terms beyond current cache_len */
        for (idx_t step = cur_steps + 1; step <= n; ++step) {
            /* next = cur * adj; adjacency entries are 0/1 */
            for (int j = 0; j < 16; ++j) next[j] = (seq_t)0;
            for (int i = 0; i < 16; ++i) {
                if (cur[i] == (seq_t)0) continue;
                /* accumulate to destinations where adj[i][j] is true */
                for (int j = 0; j < 16; ++j)
                    if (adj[i][j]) next[j] += cur[i];
            }
            /* copy next to cur for the next iteration */
            for (int j = 0; j < 16; ++j) cur[j] = next[j];

            /* sum of cur gives a(step)   (step >= 1 corresponds to n = step) */
            seq_t sum = (seq_t)0;
            for (int j = 0; j < 16; ++j) sum += cur[j];
            cache[step - 1] = sum;      /* cache index 0 corresponds to n=1 */
        }
        cur_steps = n;
        cache_len = n;
    }

    return cache[n - 1];
}
