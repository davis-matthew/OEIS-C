/*
 * A251324  Number of (n+1) X (6+1) 0..1 arrays with every 2 X 2 subblock
 *          having one or two 1's.
 * https://oeis.org/A251324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A251324 – Number of (n+1)×7 binary arrays whose every 2×2 sub‑block
    contains exactly one or two 1’s.
    
    The sequence is obtained by dynamic programming over the 128 possible
    row patterns.  A row r1 can be followed by a row r2 iff every 2×2 block
    formed by columns (c,c+1) and rows (r1,r2) has 1 or 2 ones.  An
    adjacency matrix adj[128][128] encodes this rule.

    Let dp[i] be the number of binary arrays of current length ending
    with row i.  Initially, for one row all 128 patterns are possible,
    so dp[i] = 1.  For each additional row j:

        nextdp[j] = Σ_{i} adj[i][j] * dp[i]

    After adding a row the total number of arrays of that length is
    Σ_j nextdp[j].

    The function memoises results up to the requested n.  The table
    `cache[n]` holds a(n) for 1 ≤ n ≤ MAX_N (20000).  This keeps
    subsequent evaluations fast.  All arithmetic is performed in the
    user‑supplied type `seq_t`; the code compiles as both C11 and
    C++17. */

#include <stdint.h>
#include <string.h>

/* Adjacency matrix: 1 if a[i] can precede a[j] */
static uint8_t adj[128][128] = {0};
static int adj_built = 0;

/* DP for the current length (number of rows processed) */
static seq_t dp[128] = {0};
static seq_t nextdp[128] = {0};
static idx_t L_current = 1;          /* rows processed so far */

/* Memoisation cache */
#define MAX_N        20000          /* supports n up to 20000 */
static seq_t cache[MAX_N + 1] = {0};
static idx_t cache_len = 0;         /* largest n computed */

/* Build the adjacency matrix once */
static void build_adj(void)
{
    if (adj_built) return;
    for (idx_t r1 = 0; r1 < 128; ++r1) {
        for (idx_t r2 = 0; r2 < 128; ++r2) {
            int ok = 1;
            for (idx_t c = 0; c < 6; ++c) {
                int s = ((r1 >> c)     & 1) + ((r1 >> (c + 1)) & 1)
                      + ((r2 >> c)     & 1) + ((r2 >> (c + 1)) & 1);
                if (s == 0 || s == 3 || s == 4) { ok = 0; break; }
            }
            adj[r1][r2] = (uint8_t)ok;
        }
    }
    adj_built = 1;
}

/* Initialise DP for the first row if needed */
static void init_dp(void)
{
    if (cache_len != 0) return;
    for (int i = 0; i < 128; ++i) dp[i] = 1;
    L_current = 1;
}

/* Extend the memoised values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;
    build_adj();
    init_dp();

    while (cache_len < n) {
        /* Compute nextdp from current dp */
        for (idx_t j = 0; j < 128; ++j) {
            seq_t sum = 0;
            for (idx_t i = 0; i < 128; ++i) {
                if (adj[i][j]) sum += dp[i];
            }
            nextdp[j] = sum;
        }

        /* Advance DP and length counter */
        for (idx_t j = 0; j < 128; ++j) dp[j] = nextdp[j];
        ++L_current;   /* this is now the number of rows represented */

        /* Total for arrays of length L_current rows = a(L_current-1) */
        seq_t total = 0;
        for (idx_t i = 0; i < 128; ++i) total += dp[i];

        /* Store a(n) where n = L_current-1 */
        cache[++cache_len] = total;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */
    if (n > MAX_N) {
        /* For very large n the static cache is insufficient;
           recompute without caching (relatively expensive).  */
        build_adj();
        init_dp();
        idx_t rows_needed = n + 1;   /* number of rows in the array */
        for (idx_t k = 1; k < rows_needed; ++k) {
            for (idx_t j = 0; j < 128; ++j) {
                seq_t sum = 0;
                for (idx_t i = 0; i < 128; ++i) {
                    if (adj[i][j]) sum += dp[i];
                }
                nextdp[j] = sum;
            }
            for (idx_t j = 0; j < 128; ++j) dp[j] = nextdp[j];
        }
        seq_t total = 0;
        for (idx_t i = 0; i < 128; ++i) total += dp[i];
        return total;
    }

    ensure_cache(n);
    return cache[n];
}
