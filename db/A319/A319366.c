/*
 * A319366  Number of 6 X n binary matrices such that any 2 rows have a common
 *          1.
 * https://oeis.org/A319366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{S⊆{1,…,15}} (-1)^{|S|} M_S^n, where M_S is the number
 *            of 6‑bit column vectors that do not contain both 1s in any of
 *            the pairs belonging to S
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/big_pow.h"

#define NUM_EDGES   15
#define NUM_SUBSETS (1 << NUM_EDGES)

/* List of the 15 unordered pairs of the 6 rows (0‑indexed). */
static const int edge_pair[NUM_EDGES][2] = {
    {0,1},{0,2},{0,3},{0,4},{0,5},
    {1,2},{1,3},{1,4},{1,5},
    {2,3},{2,4},{2,5},
    {3,4},{3,5},
    {4,5}
};

/* Pre‑computed for each subset U of edges:
   - col_counts[U]   : number of 6‑bit vectors that avoid all pairs in U
   - even_pop[U]     : (|U| mod 2 == 0) ?
*/
static uint8_t col_counts[NUM_SUBSETS];
static bool   even_pop[NUM_SUBSETS];
static bool   init_done = false;

/* Helper: compute the tables once. */
static void init_tables(void)
{
    /* For each subset U of edges, count admissible columns. */
    for (int U = 0; U < NUM_SUBSETS; ++U) {
        int cnt = 0;
        for (int v = 0; v < 64; ++v) {
            bool ok = true;
            /* Check all edges marked in U. */
            for (int e = 0; e < NUM_EDGES; ++e) {
                if (U & (1 << e)) {
                    int i = edge_pair[e][0];
                    int j = edge_pair[e][1];
                    if (((v >> i) & 1u) && ((v >> j) & 1u)) {
                        ok = false;
                        break;
                    }
                }
            }
            if (ok) ++cnt;
        }
        col_counts[U] = (uint8_t)cnt;
        even_pop[U]   = (__builtin_popcount((unsigned)U) & 1) == 0;
    }
    init_done = true;
}

/* Cache of base^n for bases 0..64.  Size 65. */
static seq_t pow_cache[65];

seq_t a(idx_t n)
{
    if (!init_done) init_tables();

    /* Compute base^n for bases 0..64 using big_pow. */
    seq_t exp = (seq_t)n;
    for (int b = 0; b <= 64; ++b) {
        pow_cache[b] = big_pow((seq_t)b, exp);
    }

    /* Inclusion‑exclusion over all subsets of edges. */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    for (int U = 0; U < NUM_SUBSETS; ++U) {
        seq_t term = pow_cache[col_counts[U]];
        if (even_pop[U]) {
            sum_pos += term;
        } else {
            sum_neg += term;
        }
    }
    /* The result is guaranteed to be non‑negative. */
    return sum_pos - sum_neg;
}
