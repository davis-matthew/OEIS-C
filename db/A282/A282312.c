/*
 * A282312  Number of nX4 0..1 arrays with no 1 equal to more than four of its
 *          king-move neighbors.
 * https://oeis.org/A282312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of n×4 0..1 arrays with no 1 having more than four
 *            king-move neighbors
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   check_triple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include "../utils/check_triple.h"

/* Helper tables ---------------------------------------------------------- */

static bool valid_inited = false;
static bool valid[16][16][16];

/* Initialise the validity table once. */
static void init_valid(void)
{
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            for (int c = 0; c < 16; ++c)
                valid[a][b][c] = check_triple(a, b, c);
    valid_inited = true;
}

/* Compute a(n) for n <= 18 using direct dynamic programming. */
static seq_t dp_value(idx_t n)
{
    if (!valid_inited) init_valid();

    seq_t dp[16][16];
    seq_t next[16][16];

    /* zero initialise dp */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;

    dp[0][0] = 1;

    for (idx_t row = 0; row < n; ++row) {
        /* zero initialise next */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                next[i][j] = 0;

        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b) {
                seq_t cnt = dp[a][b];
                if (cnt == 0) continue;
                for (int c = 0; c < 16; ++c) {
                    if (valid[a][b][c])
                        next[b][c] += cnt;
                }
            }

        /* copy next to dp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = next[i][j];
    }

    /* sum over all states that satisfy the final row against a zero below */
    seq_t total = 0;
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            if (valid[a][b][0])
                total += dp[a][b];

    return total;
}

/* Static cache for the first 18 terms */
static bool init_done = false;
static seq_t init[19];

/* Compute and store a(1)..a(18) */
static void compute_init(void)
{
    for (idx_t i = 1; i <= 18; ++i)
        init[i] = dp_value(i);
    init_done = true;
}

/* Main sequence function ------------------------------------------------- */

seq_t a(idx_t n)
{
    if (!init_done)
        compute_init();

    if (n <= 18)
        return init[n];

    /* recurrence of order 18 */
    seq_t last[19];             /* index 1..18 only used */
    for (int i = 1; i <= 18; ++i)
        last[i] = init[i];

    for (idx_t k = 19; k <= n; ++k) {
        seq_t next = (seq_t)(9)   * last[18]
                    + (seq_t)(43) * last[17]
                    + (seq_t)(233)* last[16]
                    - (seq_t)(369)* last[15]
                    - (seq_t)(1432)*last[14]
                    - (seq_t)(4530)*last[13]
                    + (seq_t)(2472)*last[12]
                    + (seq_t)(11204)*last[11]
                    + (seq_t)(21774)*last[10]
                    - (seq_t)(142) * last[9]
                    - (seq_t)(13268)*last[8]
                    - (seq_t)(20778)*last[7]
                    - (seq_t)(1758) * last[6]
                    + (seq_t)(3980)*last[5]
                    + (seq_t)(5134)*last[4]
                    + (seq_t)(500) * last[3]
                    + (seq_t)(68)  * last[2]
                    + (seq_t)(72)  * last[1];

        /* shift */
        for (int i = 1; i < 18; ++i)
            last[i] = last[i + 1];
        last[18] = next;
    }

    return last[18];
}
