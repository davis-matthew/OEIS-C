/*
 * A251461  Number of (n+1)X(2+1) 0..2 arrays with every 2X2 subblock summing
 *          to a prime and those sums nondecreasing in every row and column.
 * https://oeis.org/A251461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 16, u64 n <= 39, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A251461 – number of (n+1)×3 0..2 arrays with every 2×2
 * subblock summing to a prime (2,3,5,7) and those sums nondecreasing in
 * rows and columns.  The value is computed by dynamic programming
 * over consecutive rows. */

#include <stdint.h>
#include <stddef.h>

static const uint8_t PRIMES[9] = {0,0,1,1,0,1,0,1,0}; /* 2,3,5,7 are prime */

typedef struct {
    int prev;          /* index of the previous row (0..26) */
    int curr;          /* index of the current row (0..26) */
    uint8_t s0;        /* sum of columns 0‑1 for this pair */
    uint8_t s1;        /* sum of columns 1‑2 for this pair */
} Pair;

seq_t a(idx_t n)
{
    /* generate all 27 possible rows */
    int rows[27][3];
    int r = 0;
    for (int x = 0; x <= 2; ++x)
        for (int y = 0; y <= 2; ++y)
            for (int z = 0; z <= 2; ++z)
                rows[r][0] = x, rows[r][1] = y, rows[r][2] = z, ++r;

    /* enumerate all valid pairs of consecutive rows */
    Pair pairs[729];      /* maximum 27×27 */
    int pair_cnt = 0;
    for (int i = 0; i < 27; ++i)
        for (int j = 0; j < 27; ++j) {
            uint8_t s0 = rows[i][0] + rows[i][1] + rows[j][0] + rows[j][1];
            uint8_t s1 = rows[i][1] + rows[i][2] + rows[j][1] + rows[j][2];
            if (PRIMES[s0] && PRIMES[s1] && s0 <= s1) {
                pairs[pair_cnt].prev = i;
                pairs[pair_cnt].curr = j;
                pairs[pair_cnt].s0   = s0;
                pairs[pair_cnt].s1   = s1;
                ++pair_cnt;
            }
        }

    /* build adjacency lists: for each parent row index, list all pairs
       that start with that parent. */
    int adj[27][28];      /* adj[p] = list of pair indices with prev == p */
    int adj_sz[27];
    for (int i = 0; i < 27; ++i) adj_sz[i] = 0;
    for (int idx = 0; idx < pair_cnt; ++idx)
        adj[pairs[idx].prev][adj_sz[pairs[idx].prev]++] = idx;

    /* dynamic programming over the sequence of pairs */
    seq_t dp[729] = {0}, newdp[729] = {0};
    for (int i = 0; i < pair_cnt; ++i)
        dp[i] = 1;  /* any valid first pair can occur */

    for (idx_t step = 1; step < n; ++step) {
        /* zero newdp */
        for (int i = 0; i < pair_cnt; ++i) newdp[i] = 0;
        /* transition */
        for (int a = 0; a < pair_cnt; ++a) {
            if (dp[a] == 0) continue;
            int c = pairs[a].curr;
            uint8_t s0_a = pairs[a].s0;
            uint8_t s1_a = pairs[a].s1;
            int *b_list = adj[c];
            int b_sz = adj_sz[c];
            for (int b_i = 0; b_i < b_sz; ++b_i) {
                int b = b_list[b_i];
                if (pairs[b].s0 >= s0_a && pairs[b].s1 >= s1_a)
                    newdp[b] += dp[a];
            }
        }
        /* swap dp and newdp */
        for (int i = 0; i < pair_cnt; ++i) dp[i] = newdp[i];
    }

    /* sum all ways after processing all rows */
    seq_t result = 0;
    for (int i = 0; i < pair_cnt; ++i)
        result += dp[i];
    return result;
}
