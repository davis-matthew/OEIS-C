/*
 * A297454  Number of n X 5 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 0 or 2 neighboring 1's.
 * https://oeis.org/A297454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A297454:  Number of n × 5 0/1 arrays where each 1 has
 * an even number (0 or 2) of horizontally or diagonally adjacent
 * 1‑neighbours.  The computation uses a breadth‑first DP over
 * pairs of consecutive rows.  At each step we keep the last two
 * rows (prevprev, prev) and extend by a new row `curr`.  A triple
 * (prevprev, prev, curr) is admissible iff every 1 in the middle
 * row `prev` has 0 or 2 neighbours among
 *   - its left/right cells in `prev`
 *   - the cells diagonally left/right in `prevprev` and `curr`
 * (vertical neighbours are ignored).  The state is reduced to
 * 32 × 32 = 1024 possibilities, so the algorithm runs in O(n·32³)
 * time and is well below the 2‑second limit for n up to 10 000. */

#include <stdbool.h>

static bool valid_triplet[32][32][32];   /* pre‑computed admissibility */

/*--- Build the admissibility table -------------------------------------*/
static void build_valid_triplets(void)
{
    for (int pprev = 0; pprev < 32; ++pprev)          /* row i-2 */
        for (int prv   = 0; prv   < 32; ++prv)          /* row i-1 */
            for (int cur  = 0; cur  < 32; ++cur) {       /* row i   */
                bool ok = true;
                /* Check every 1 in the middle row `prv` */
                for (int j = 0; j < 5 && ok; ++j)
                    if (prv & (1u << j)) {
                        unsigned cnt = 0;
                        /* horizontal neighbours in `prv` */
                        if (j > 0   && (prv & (1u << (j - 1)))) cnt++;
                        if (j < 4   && (prv & (1u << (j + 1)))) cnt++;
                        /* diagonal neighbours in `pprev` */
                        if (j > 0   && (pprev & (1u << (j - 1)))) cnt++;
                        if (j < 4   && (pprev & (1u << (j + 1)))) cnt++;
                        /* diagonal neighbours in `cur` */
                        if (j > 0   && (cur   & (1u << (j - 1)))) cnt++;
                        if (j < 4   && (cur   & (1u << (j + 1)))) cnt++;
                        if (cnt != 0 && cnt != 2) ok = false;
                    }
                valid_triplet[pprev][prv][cur] = ok;
            }
}

/*--- Sequence a(n) -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset starts at 1 */

    /* Build validity table once (thread‑safe in a single‐threaded context). */
    static bool ready = false;
    if (!ready) { build_valid_triplets(); ready = true; }

    /* DP tables: dp[pprev][prv]  = number of arrays where the
     * last two processed rows are (pprev, prv).                */
    seq_t dp[32][32] = {{0}}, nextdp[32][32] = {{0}};

    /* initialise with the first row: prevprev = 0 (sentinel) */
    for (int r = 0; r < 32; ++r) dp[0][r] = (seq_t)1;

    /* iterate over remaining rows (step counts already processed rows) */
    for (idx_t step = 1; step < n; ++step) {
        /* clear nextdp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                nextdp[i][j] = (seq_t)0;

        /* transition */
        for (int pprev = 0; pprev < 32; ++pprev)
            for (int prv = 0; prv < 32; ++prv) {
                seq_t ways = dp[pprev][prv];
                if (ways == (seq_t)0) continue;
                for (int cur = 0; cur < 32; ++cur)
                    if (valid_triplet[pprev][prv][cur])
                        nextdp[prv][cur] = nextdp[prv][cur] + ways;
            }

        /* swap dp and nextdp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = nextdp[i][j];
    }

    /* final check: the last row `prv` must satisfy the rule with
     * a trailing sentinel row of zeros.                               */
    seq_t total = (seq_t)0;
    for (int pprev = 0; pprev < 32; ++pprev)
        for (int prv = 0; prv < 32; ++prv)
            if (valid_triplet[pprev][prv][0])
                total = total + dp[pprev][prv];

    return total;
}
