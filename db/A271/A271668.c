/*
 * A271668  Triangle read by rows. The first column is A000217(n+1). From the
 *          second row we apply - A002262(n) for the following terms of the
 *          row.
 * https://oeis.org/A271668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((r+1)(r+2)/2) - (j(j-1)/2), where r =
 *            floor((sqrt(1+8n)-1)/2) and j = n - r(r+1)/2. This implements
 *            the triangle described in A271668
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static idx_t isqrt_u128(unsigned __int128 x)
{
    unsigned long long r = (unsigned long long)sqrtl((long double)x);
    /* adjust to exact integer square root */
    while ((unsigned __int128)(r + 1) * (r + 1) <= x) ++r;
    while ((unsigned __int128)r * r > x) --r;
    return r;
}

seq_t a(idx_t n)
{
    /* r = floor((sqrt(1+8n)-1)/2) */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    idx_t root = isqrt_u128(val);
    idx_t r = (root - 1) / 2;

    /* start index of row r */
    unsigned __int128 start128 = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t start = (idx_t)start128;

    idx_t j = n - start;          /* column within the row */

    unsigned __int128 rowFirst128 = ((unsigned __int128)(r + 1) *
                                     (r + 2)) / 2;
    unsigned __int128 sub128 = ((unsigned __int128)j *
                                (j - 1)) / 2;

    return (seq_t)(rowFirst128 - sub128);
}
