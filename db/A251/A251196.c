/*
 * A251196  Number of (n+1)X(3+1) 0..1 arrays with no 2X2 subblock having the
 *          minimum of its diagonal elements less than the absolute difference
 *          of its antidiagonal elements.
 * https://oeis.org/A251196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A251196
   Computation method:
   A  (n+1)×4 binary array satisfies the 2×2 subblock constraint iff
   for every adjacent pair of rows r and s the following holds for
   columns j=0,1,2:
      let a = bit j of r,  b = bit j+1 of r
          c = bit j of s,  d = bit j+1 of s
      the block (a b; c d) is allowed iff
          (b == c)  OR  (a == 1 && d == 1).
   Thus the set of all 16 possible rows forms a directed graph
   with an edge r→s if the pair (r,s) is allowed.  
   Counting all (n+1)×4 arrays is counting all walks of length n in
   this graph, starting from any row and ending at any row.
   The number a(n) is therefore the sum of all entries of the
   matrix Aⁿ, where A is the 16×16 adjacency matrix of the graph.
   We compute it by dynamic programming in O(256·n) operations,
   which easily meets the requirements for n≤10000. */

#include <stdbool.h>

static bool adjacency_built = false;
static unsigned char adjacency[16][16] = {{0}};

static void build_adjacency(void)
{
    if (adjacency_built) return;
    for (unsigned int r = 0; r < 16; ++r) {
        for (unsigned int s = 0; s < 16; ++s) {
            bool ok = true;
            for (unsigned int j = 0; j < 3; ++j) {
                unsigned int a = (r >> j)       & 1;
                unsigned int b = (r >> (j + 1)) & 1;
                unsigned int c = (s >> j)       & 1;
                unsigned int d = (s >> (j + 1)) & 1;
                if (b != c && !(a && d)) { ok = false; break; }
            }
            adjacency[r][s] = ok ? 1 : 0;
        }
    }
    adjacency_built = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    build_adjacency();

    seq_t dp[16];
    seq_t ndp[16];
    for (unsigned int i = 0; i < 16; ++i) dp[i] = (seq_t)1;   /* all starting rows */

    for (idx_t step = 0; step < n; ++step) {
        for (unsigned int i = 0; i < 16; ++i) ndp[i] = (seq_t)0;
        for (unsigned int r = 0; r < 16; ++r) {
            if (dp[r] == 0) continue;
            for (unsigned int s = 0; s < 16; ++s) if (adjacency[r][s]) {
                ndp[s] += dp[r];
            }
        }
        for (unsigned int i = 0; i < 16; ++i) dp[i] = ndp[i];
    }

    seq_t total = (seq_t)0;
    for (unsigned int i = 0; i < 16; ++i) total += dp[i];
    return total;
}
