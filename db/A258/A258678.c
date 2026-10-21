/*
 * A258678  Number of (n+2)X(5+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum equal to the antidiagonal sum.
 * https://oeis.org/A258678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 12, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A258678: Number of (n+2)×7 binary arrays with no 3×3 subblock
 * where the sum of the main diagonal equals the sum of the anti‑diagonal.
 *
 * This implementation builds a directed graph on the 2^7 = 128 possible
 * 7‑bit rows.  An edge u→v exists iff rows u and v can appear consecutively
 * in the array; that is, for all k=0…4 the differences
 *   (u_k – u_{k+2})  and  (v_k – v_{k+2})
 * are unequal (otherwise a forbidden 3×3 block would appear).
 *
 * The array rows form two independent chains: the even‑indexed rows and
 * the odd‑indexed rows.  Let L_even = ceil((n+2)/2) and
 *     L_odd  = floor((n+2)/2).
 * Each chain is counted by counting walks of length L-1 in the graph.
 * The total number is the product of the counts for the two chains.
 *
 * The code works for any seq_t that supports +,-,*,/,% and assignment.
 * It avoids using memcpy or memset on seq_t objects to stay safe for
 * arbitrary‑precision types.                          */

#include <stdint.h>
#include <stddef.h>

static uint8_t neigh[128][128];
static uint8_t deg[128];
static int adjacency_built = 0;

static void build_adjacency(void)
{
    if (adjacency_built) return;
    for (int u = 0; u < 128; ++u) {
        for (int v = 0; v < 128; ++v) {
            int ok = 1;
            for (int k = 0; k < 5; ++k) {
                int ru = ((u >> k) & 1) - ((u >> (k + 2)) & 1);
                int rv = ((v >> k) & 1) - ((v >> (k + 2)) & 1);
                if (ru == rv) { ok = 0; break; }
            }
            if (ok) {
                neigh[u][deg[u]++] = (uint8_t)v;
            }
        }
    }
    adjacency_built = 1;
}

seq_t a(idx_t n)
{
    build_adjacency();

    idx_t rows = n + 2;                    /* total rows in the array   */
    idx_t L_even = (rows + 1) / 2;         /* ceil((n+2)/2)             */
    idx_t L_odd  = rows / 2;               /* floor((n+2)/2)            */
    idx_t L_max  = (L_even > L_odd) ? L_even : L_odd;

    seq_t dp_cur[128];
    seq_t dp_nxt[128];

    /* initialise dp_cur with 1 for all possible first rows of a chain */
    for (int i = 0; i < 128; ++i) dp_cur[i] = (seq_t)1;

    seq_t total_even = 0, total_odd = 0;
    if (L_even == 1) total_even = 128;
    if (L_odd  == 1) total_odd  = 128;

    for (idx_t step = 2; step <= L_max; ++step) {
        /* zero dp_nxt */
        for (int i = 0; i < 128; ++i) dp_nxt[i] = (seq_t)0;

        /* transition */
        for (int u = 0; u < 128; ++u) {
            seq_t val = dp_cur[u];
            if (!val) continue;
            int d = deg[u];
            uint8_t *nlist = neigh[u];
            for (int e = 0; e < d; ++e) {
                dp_nxt[nlist[e]] += val;
            }
        }

        /* copy dp_nxt to dp_cur */
        for (int i = 0; i < 128; ++i) dp_cur[i] = dp_nxt[i];

        /* accumulate total for this chain length */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < 128; ++i) sum += dp_cur[i];
        if (step == L_even) total_even = sum;
        if (step == L_odd)  total_odd  = sum;
    }

    return total_even * total_odd;
}
