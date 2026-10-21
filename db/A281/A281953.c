/*
 * A281953  Number of nX6 0..1 arrays with no element unequal to more than
 *          four of its king-move neighbors and with new values introduced in
 *          order 0 sequentially upwards.
 * https://oeis.org/A281953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays with first cell = 0 such that
 *            no element differs from more than four of its king‑move
 *            neighbors
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Dimensions */
#define ROWS 64                 /* 6‑bit rows : 0 … 63          */
#define STATE 65                /* rows plus sentinel NONE (64)  */
#define NONE 64                 /* sentinel for “no neighbour row” */

/* Pre‑computed tables */
static bool  valid[STATE][ROWS][STATE];               /* validity of (prev,curr,next) */
static unsigned char next_list[STATE][ROWS][ROWS];    /* list of next rows for each (prev,curr) */
static unsigned char next_cnt[STATE][ROWS];           /* number of entries in next_list */

/* Table having been initialised once */
static bool tables_initialised = false;

/* --------------------------------------------------------------------- */
/*  Verify whether row curr satisfies the local constraint when its up
    neighbours are given by prev and its down neighbours by next.
    Missing neighbours are ignored.                                         */
static bool verify_row(uint64_t curr, uint64_t prev, uint64_t next)
{
    for (int col = 0; col < 6; ++col) {
        uint64_t cell = (curr >> col) & 1ULL;
        int diff = 0;

        /* left neighbour */
        if (col > 0)
            diff += (cell ^ ((curr >> (col - 1)) & 1ULL)) != 0;
        /* right neighbour */
        if (col < 5)
            diff += (cell ^ ((curr >> (col + 1)) & 1ULL)) != 0;

        /* up neighbour */
        if (prev != NONE)
            diff += (cell ^ ((prev >> col) & 1ULL)) != 0;
        /* down neighbour */
        if (next != NONE)
            diff += (cell ^ ((next >> col) & 1ULL)) != 0;

        /* up-left */
        if (prev != NONE && col > 0)
            diff += (cell ^ ((prev >> (col - 1)) & 1ULL)) != 0;
        /* up-right */
        if (prev != NONE && col < 5)
            diff += (cell ^ ((prev >> (col + 1)) & 1ULL)) != 0;
        /* down-left */
        if (next != NONE && col > 0)
            diff += (cell ^ ((next >> (col - 1)) & 1ULL)) != 0;
        /* down-right */
        if (next != NONE && col < 5)
            diff += (cell ^ ((next >> (col + 1)) & 1ULL)) != 0;

        if (diff > 4)
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Build all tables only once                    */
static void build_tables(void)
{
    /* ----- Validity table ----- */
    for (int prev = 0; prev < STATE; ++prev) {
        for (int curr = 0; curr < ROWS; ++curr) {
            for (int next = 0; next < STATE; ++next) {
                valid[prev][curr][next] =
                    verify_row((uint64_t)curr, (uint64_t)prev, (uint64_t)next);
            }
        }
    }

    /* ----- Transition lists ----- */
    for (int prev = 0; prev < STATE; ++prev) {
        for (int curr = 0; curr < ROWS; ++curr) {
            unsigned char cnt = 0;
            for (int next = 0; next < ROWS; ++next) {   /* next row is always 0 … 63 */
                if (valid[prev][curr][next]) {
                    next_list[prev][curr][cnt++] = (unsigned char)next;
                }
            }
            next_cnt[prev][curr] = cnt;
        }
    }

    tables_initialised = true;
}

/* --------------------------------------------------------------------- */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (!tables_initialised)
        build_tables();

    /* dp[prev][curr] – number of ways arriving at state (prev,curr) */
    static seq_t dp[STATE][ROWS];
    static seq_t ndp[STATE][ROWS];

    /* initialise all to zero */
    for (int p = 0; p < STATE; ++p)
        for (int c = 0; c < ROWS; ++c)
            dp[p][c] = 0;

    /* first row – prev = NONE, curr any 6‑bit row with leftmost bit 0   */
    for (int curr = 0; curr < ROWS; ++curr) {
        if ((curr & 1U) == 0)          /* leftmost bit == 0 */
            dp[NONE][curr] = 1;
    }

    /* iterate rows 2 … n */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear ndp */
        for (int p = 0; p < STATE; ++p)
            for (int c = 0; c < ROWS; ++c)
                ndp[p][c] = 0;

        for (int prev = 0; prev < STATE; ++prev) {
            for (int curr = 0; curr < ROWS; ++curr) {
                seq_t val = dp[prev][curr];
                if (!val)
                    continue;

                unsigned char cnt = next_cnt[prev][curr];
                unsigned char *list = next_list[prev][curr];
                for (unsigned char k = 0; k < cnt; ++k) {
                    unsigned char next = list[k];
                    ndp[curr][next] += val;
                }
            }
        }

        /* copy ndp into dp for next iteration      */
        for (int p = 0; p < STATE; ++p)
            for (int c = 0; c < ROWS; ++c)
                dp[p][c] = ndp[p][c];
    }

    /* final row – validate with next = NONE */
    seq_t result = 0;
    for (int prev = 0; prev < STATE; ++prev) {
        for (int curr = 0; curr < ROWS; ++curr) {
            if (!dp[prev][curr])
                continue;
            if (valid[prev][curr][NONE])
                result += dp[prev][curr];
        }
    }
    return result;
}
