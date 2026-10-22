/*
 * A388058  a(n) is the permanent of a 4 X 4 matrix M(n) whose generic element
 *          is given by binomial(2*n + i, n + j) with i, j = 0..3.
 * https://oeis.org/A388058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = permanent of a 4×4 matrix M(n) with M[i][j] = binom(2*n +
 *            i, n + j) (i, j = 0..3)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

static const int perm[24][4] = {
    {0,1,2,3},
    {0,1,3,2},
    {0,2,1,3},
    {0,2,3,1},
    {0,3,1,2},
    {0,3,2,1},
    {1,0,2,3},
    {1,0,3,2},
    {1,2,0,3},
    {1,2,3,0},
    {1,3,0,2},
    {1,3,2,0},
    {2,0,1,3},
    {2,0,3,1},
    {2,1,0,3},
    {2,1,3,0},
    {2,3,0,1},
    {2,3,1,0},
    {3,0,1,2},
    {3,0,2,1},
    {3,1,0,2},
    {3,1,2,0},
    {3,2,0,1},
    {3,2,1,0}
};

seq_t a(idx_t n)
{
    seq_t M[4][4];
    for (idx_t i = 0; i < 4; ++i) {
        idx_t row = 2*n + i;
        for (idx_t j = 0; j < 4; ++j) {
            idx_t col = n + j;
            M[i][j] = binom(row, col);
        }
    }

    seq_t sum = 0;
    for (int p = 0; p < 24; ++p) {
        seq_t prod = 1;
        for (int i = 0; i < 4; ++i) {
            prod *= M[i][perm[p][i]];
        }
        sum += prod;
    }
    return sum;
}
