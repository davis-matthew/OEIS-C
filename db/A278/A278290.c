/*
 * A278290  Number of neighbors of each new term in a square array read by
 *          antidiagonals.
 * https://oeis.org/A278290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of already-placed neighbors of the entry with *
 *            index n in the antidiagonal filling of an infinite square array.
 *            * The array is indexed starting at (1,1). For n ≥ 1, a(n) is
 *            computed * by locating the coordinates (i,j) of entry n, then
 *            counting the 8 * neighbouring positions (i±1,j±...
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   coords_from_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/coords_from_idx.h"
static inline idx_t idx_from_coords(idx_t i, idx_t j)
{
    idx_t s = i + j;
    unsigned __int128 prod = (unsigned __int128)(s - 2) * (s - 1);
    idx_t pre = (idx_t)(prod / 2);
    return pre + i;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t i, j;
    coords_from_idx(n, &i, &j);
    const int di[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dj[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    seq_t cnt = 0;
    for (int k = 0; k < 8; ++k)
    {
        idx_t ni = i + (idx_t)di[k];
        idx_t nj = j + (idx_t)dj[k];
        if (ni >= 1 && nj >= 1)
        {
            idx_t neigh = idx_from_coords(ni, nj);
            if (neigh < n)
                ++cnt;
        }
    }
    return cnt;
}
