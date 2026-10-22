/*
 * A300876  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 4
 *          or 5 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A300876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   dynamic‑programming over rows, testing the
 *            “adjacency‑equality‑count” rule for every cell. The first row’s
 *            leftmost element must be 0. The program uses a sentinel value
 *            (8) to represent a nonexistent row so that the upper and lower
 *            boundaries are handled uniformly. a(1) = 4, a(2) = 18, a(3) =
 *            52,...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define ROWS 9                /* 8 real row patterns + sentinel  */
#define SENTINEL 8

/* true if the triple (prev2,row,cur) satisfies the constraint for the
   middle row (row) */
static bool triple_ok(int prev2, int row, int cur)
{
    if (row == SENTINEL)       /* no middle row – nothing to check */
        return true;

    for (int col = 0; col < 3; ++col)
    {
        int val   = (row >> col) & 1;
        int cnt   = 0;

        /* left neighbour */
        if (col > 0) {
            int v = (row >> (col - 1)) & 1;
            if (v == val) ++cnt;
        }
        /* right neighbour */
        if (col < 2) {
            int v = (row >> (col + 1)) & 1;
            if (v == val) ++cnt;
        }
        /* up neighbour */
        if (prev2 != SENTINEL) {
            int v = (prev2 >> col) & 1;
            if (v == val) ++cnt;
        }
        /* up‑left neighbour */
        if (prev2 != SENTINEL && col > 0) {
            int v = (prev2 >> (col - 1)) & 1;
            if (v == val) ++cnt;
        }

        /* down neighbour */
        if (cur != SENTINEL) {
            int v = (cur >> col) & 1;
            if (v == val) ++cnt;
        }
        /* down‑right neighbour */
        if (cur != SENTINEL && col < 2) {
            int v = (cur >> (col + 1)) & 1;
            if (v == val) ++cnt;
        }

        if (cnt == 3 || cnt == 6)
            return false;
    }
    return true;
}

/* Pre‑compute all allowed triples */
static bool allowed[ROWS][ROWS][ROWS];
static bool allowed_ready = false;

static void init_allowed(void)
{
    for (int p2 = 0; p2 < ROWS; ++p2)
        for (int p1 = 0; p1 < ROWS; ++p1)
            for (int cur = 0; cur < ROWS; ++cur)
                allowed[p2][p1][cur] = triple_ok(p2, p1, cur);
    allowed_ready = true;
}

seq_t a(idx_t n)
{
    if (!allowed_ready) init_allowed();

    const size_t STATE = ROWS * ROWS;          /* 81 states */
    seq_t dpPrev[STATE];
    seq_t dpCur[STATE];

    /* initialise dpPrev for the first row (upper‑left cell fixed to 0) */
    for (size_t i = 0; i < STATE; ++i) dpPrev[i] = 0;
    for (int r = 0; r < 8; ++r)          /* actual rows 0..7 */
        if ((r & 1) == 0)               /* bit 0 (leftmost) is 0 */
            dpPrev[(size_t)r * ROWS + SENTINEL] = 1;

    /* process rows 2 … n */
    for (idx_t row = 2; row <= n; ++row)
    {
        for (size_t i = 0; i < STATE; ++i) dpCur[i] = 0;

        for (int prev1 = 0; prev1 < ROWS; ++prev1)
            for (int prev2 = 0; prev2 < ROWS; ++prev2)
            {
                seq_t val = dpPrev[(size_t)prev1 * ROWS + prev2];
                if (val == 0) continue;

                for (int cur = 0; cur < 8; ++cur)          /* real rows */
                    if (allowed[prev2][prev1][cur])
                        dpCur[(size_t)cur * ROWS + prev1] += val;
            }

        /* copy dpCur into dpPrev for next iteration */
        for (size_t i = 0; i < STATE; ++i) dpPrev[i] = dpCur[i];
    }

    /* final evaluation: last row has no row below (sentinel) */
    seq_t result = 0;
    for (int last = 0; last < ROWS; ++last)
        for (int prev = 0; prev < ROWS; ++prev)
        {
            seq_t val = dpPrev[(size_t)last * ROWS + prev];
            if (val == 0) continue;
            if (allowed[prev][last][SENTINEL])
                result += val;
        }

    return result;
}
