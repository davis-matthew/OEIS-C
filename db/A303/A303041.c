/*
 * A303041  Number of 3Xn 0..1 arrays with every element equal to 0, 1, 3, 4
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1..12) = [4,6,14,28,56,150,360,828,1906,4628,10972,25850] a(n)
 *            = 2*a(n-1)+2*a(n-2)-2*a(n-3)+6*a(n-4)-16*a(n-5)-11*a(n-6)
 *            +14*a(n-7)-4*a(n-8)+16*a(n-9)+4*a(n-10)-8*a(n-11) for n>12
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const seq_t init_vals[12] = {
    4ULL, 6ULL, 14ULL, 28ULL, 56ULL, 150ULL, 360ULL,
    828ULL, 1906ULL, 4628ULL, 10972ULL, 25850ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside offset, we return 0 (unused) */
    if (n <= 12) return init_vals[n-1];

    /* Buffer holds the last 11 values: prev[0]=a(k-1), prev[1]=a(k-2), ..., prev[10]=a(k-11) */
    seq_t prev[11];
    /* initialise from the last 11 precomputed terms */
    for (int i = 0; i < 11; ++i)
        prev[i] = init_vals[11 - i];

    seq_t cur = 0;
    for (idx_t k = 13; k <= n; ++k) {
        /* compute positive part */
        seq_t pos =
            2 * prev[0] + 2 * prev[1] +
            6 * prev[3] + 14 * prev[6] +
            16 * prev[8] + 4 * prev[9];
        /* compute negative part */
        seq_t neg =
            2 * prev[2] + 16 * prev[4] + 11 * prev[5] +
            4 * prev[7] + 8 * prev[10];
        /* result = pos - neg (pos > neg for all k >= 13) */
        cur = pos - neg;

        /* shift buffer: drop oldest, insert current */
        for (int i = 10; i > 0; --i)
            prev[i] = prev[i-1];
        prev[0] = cur;
    }
    return cur;
}
