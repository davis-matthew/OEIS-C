/*
 * A268787  Number of nX6 binary arrays with some element plus some
 *          horizontally, vertically or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A268787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A268787
    Number of n×6 binary arrays with exactly one adjacency pair
    (two neighboring 1’s) horizontally, vertically or anti‑diagonally. */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*  External integer types supplied by the build.            */
/* #define idx_t   uint64_t   (example)                        */
/* #define seq_t   uint64_t   (example)                        */

static unsigned char popcnt[64];
static unsigned char horiz[64];
static unsigned char addAdj[64][64];

static int static_inited = 0;

/*  Pre‑compute tables used by the DP.  */
static void init_tables(void)
{
    if (static_inited) return;
    for (int i = 0; i < 64; ++i) {
        unsigned char c = 0;
        unsigned int x = i;
        while (x) { x &= x - 1; ++c; }
        popcnt[i] = c;
    }
    for (int m = 0; m < 64; ++m)
        horiz[m] = popcnt[ m & (m >> 1) ];

    for (int pm = 0; pm < 64; ++pm)
        for (int cm = 0; cm < 64; ++cm)
            addAdj[pm][cm] =
                (unsigned char)(horiz[cm] + popcnt[pm & cm] + popcnt[(pm >> 1) & cm]);

    static_inited = 1;
}

/*  Global cache of computed sequence values.  */
static seq_t *seq_cache = NULL;      /* sequence values a(1..)  */
static idx_t cache_size   = 0;       /* number of entries allocated  */
static idx_t computed_n   = 0;       /* largest n for which the cache is correct  */

static seq_t cur_dp[64][2];
static seq_t nxt_dp[64][2];

/*  Ensure that a(i) is known for all i <= n.  */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;
    if (cache_size < n+1) {
        seq_t *tmp = (seq_t*)realloc(seq_cache, (n+1) * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        seq_cache = tmp;
        cache_size = n;
    }
    if (computed_n >= n) return;      /* already done */

    init_tables();

    /*  First time: initialise for n = 1  */
    if (computed_n == 0) {
        memset(cur_dp, 0, sizeof(cur_dp));
        for (int m = 0; m < 64; ++m) {
            unsigned char h = horiz[m];
            if (h <= 1) cur_dp[m][h] = 1;
        }
        /*  a(1)  */
        seq_t sum = 0;
        for (int m = 0; m < 64; ++m) sum += cur_dp[m][1];
        seq_cache[1] = sum;
        computed_n = 1;
    }

    /*  Grow the cache up to n  */
    for (idx_t row = computed_n + 1; row <= n; ++row) {
        memset(nxt_dp, 0, sizeof(nxt_dp));
        for (int pm = 0; pm < 64; ++pm) {
            for (int k = 0; k < 2; ++k) {
                seq_t val = cur_dp[pm][k];
                if (!val) continue;
                for (int cm = 0; cm < 64; ++cm) {
                    unsigned char add = addAdj[pm][cm];
                    int nk = k + add;
                    if (nk < 2) {
                        nxt_dp[cm][nk] += val;
                    }
                }
            }
        }
        /* copy nxt into cur */
        for (int i = 0; i < 64; ++i) {
            cur_dp[i][0] = nxt_dp[i][0];
            cur_dp[i][1] = nxt_dp[i][1];
        }
        /* store a(row) */
        seq_t sum = 0;
        for (int m = 0; m < 64; ++m) sum += cur_dp[m][1];
        seq_cache[row] = sum;
    }
    computed_n = n;
}

/*  Public function required by the test harness.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_cache(n);
    return seq_cache[n];
}
