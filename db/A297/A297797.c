/*
 * A297797  Number of nX3 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2, 3 or 5 neighboring 1s.
 * https://oeis.org/A297797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 0..1 arrays in which every 1 has * 2, 3 or
 *            5 neighbouring 1s among the 6 cells that are * horizontally or
 *            diagonally adjacent (no vertical adjacency). * Initial values:
 *            a(1)=1, a(2)=11, a(3)=37, a(4)=108, a(5)=533
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define MASKS 8     /* 3‑bit masks: 0 … 7 */

/* Pre‑computed table of validity for triples (above, current, below) */
static bool valid_table[MASKS][MASKS][MASKS] = {0};
static bool valid_table_initialized = false;

/* Test whether the row represented by 'head' is valid when
 * surrounded by rows 'prev' (above) and 'next' (below).
 * Only horizontal and diagonal neighbours are considered. */
static bool is_valid_row(idx_t prev, idx_t head, idx_t next)
{
    for (idx_t col = 0; col < 3; ++col) {
        if (((head >> col) & 1U) == 0) continue;   /* only care about 1's */

        int neigh = 0;

        /* same row neighbours */
        if (col > 0     && ((head >> (col - 1)) & 1U)) ++neigh;   /* left  */
        if (col < 2     && ((head >> (col + 1)) & 1U)) ++neigh;   /* right */

        /* diagonal neighbours (up‑left/up‑right) */
        if (col > 0     && ((prev >> (col - 1)) & 1U)) ++neigh;   /* up-left  */
        if (col < 2     && ((prev >> (col + 1)) & 1U)) ++neigh;   /* up-right */

        /* diagonal neighbours (down‑left/down‑right) */
        if (col > 0     && ((next >> (col - 1)) & 1U)) ++neigh;   /* down-left */
        if (col < 2     && ((next >> (col + 1)) & 1U)) ++neigh;   /* down-right */

        if (!(neigh == 2 || neigh == 3 || neigh == 5)) return false;
    }
    return true;
}

/* Initialise the validity table once. */
static void init_valid_table(void)
{
    if (valid_table_initialized) return;
    for (idx_t p2 = 0; p2 < MASKS; ++p2)
        for (idx_t p1 = 0; p1 < MASKS; ++p1)
            for (idx_t cur = 0; cur < MASKS; ++cur)
                valid_table[p2][p1][cur] = is_valid_row(p2, p1, cur);
    valid_table_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence offset is 1 */

    init_valid_table();

    /* dp_prev[p2][p1] = number of ways after processing some rows,
     * ending with the top two rows having masks p2 (above) and p1 (current). */
    seq_t dp_prev[MASKS][MASKS] = {0};
    seq_t dp_cur [MASKS][MASKS] = {0};

    /* Before the first row, all above rows are zero. */
    dp_prev[0][0] = 1;

    for (idx_t row = 1; row <= n; ++row) {

        /* reset dp_cur to zero using assignment (not memset) */
        for (idx_t p2 = 0; p2 < MASKS; ++p2)
            for (idx_t p1 = 0; p1 < MASKS; ++p1)
                dp_cur[p2][p1] = 0;

        for (idx_t p2 = 0; p2 < MASKS; ++p2) {
            for (idx_t p1 = 0; p1 < MASKS; ++p1) {
                seq_t ways = dp_prev[p2][p1];
                if (!ways) continue;          /* no ways for this state */

                for (idx_t cur = 0; cur < MASKS; ++cur) {
                    if (!valid_table[p2][p1][cur]) continue;
                    dp_cur[p1][cur] += ways;
                }
            }
        }

        /* copy dp_cur into dp_prev for next iteration */
        for (idx_t p2 = 0; p2 < MASKS; ++p2)
            for (idx_t p1 = 0; p1 < MASKS; ++p1)
                dp_prev[p2][p1] = dp_cur[p2][p1];
    }

    /* Final validation: the last (bottom) row must be valid with
     * a zero mask below it. */
    seq_t total = 0;
    for (idx_t p2 = 0; p2 < MASKS; ++p2)
        for (idx_t p1 = 0; p1 < MASKS; ++p1) {
            if (!valid_table[p2][p1][0]) continue;
            total += dp_prev[p2][p1];
        }

    return total;
}
