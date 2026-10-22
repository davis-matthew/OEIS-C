/*
 * A400761  Product of the 8 nearest neighbors of n in the square spiral.
 * https://oeis.org/A400761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = product of the eight nearest neighbors of n in the square
 *            spiral. The square spiral starts with 1 at the origin, then
 *            proceeds right, up, left, down with layer lengths 1,2,3,4,... .
 *            This implementation maps n to (x,y) coordinates, then finds each
 *            neighbor's coordinates, converts them back...
 * Range:     u8 none, u16 none, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   value_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/value_at.h"

static inline idx_t isqrt_idx_t(idx_t n)
{
    idx_t low = 0, high = ((idx_t)1 << 32);   /* 2^32 > sqrt(UINT64_MAX) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * mid <= n) low = mid + 1;
        else high = mid;
    }
    return low - 1;
}

/* Map index n (n>=1) to its Cartesian coordinates (x,y) with the center (1) at (0,0). */
static inline void pos_of(idx_t n, int64_t *px, int64_t *py)
{
    if (n == 1) {
        *px = 0;
        *py = 0;
        return;
    }

    /* Determine the layer (ring) L containing n. */
    idx_t t = isqrt_idx_t(n);          /* floor sqrt(n) */
    if ((unsigned __int128)t * t < n) t++;   /* ceil sqrt(n) */
    if ((t & 1U) == 0) t++;           /* make it odd */
    idx_t L = (t - 1) / 2;            /* ring number */

    idx_t start = ((2 * L - 1) * (2 * L - 1)) + 1;   /* first number in this ring */
    idx_t d = n - start;            /* offset within ring (0 … 8L-1) */
    idx_t side = 2 * L;             /* number of steps per side */

    int64_t x, y;
    if (d < side) {                         /* right side, moving up */
        x = L;
        y = -(int64_t)(L - 1) + (int64_t)d;
    } else if (d < 2 * side) {              /* top side, moving left */
        d -= side;
        x = (int64_t)(L - 1) - (int64_t)d;
        y = L;
    } else if (d < 3 * side) {              /* left side, moving down */
        d -= 2 * side;
        x = -(int64_t)L;
        y = (int64_t)(L - 1) - (int64_t)d;
    } else {                                /* bottom side, moving right */
        d -= 3 * side;
        x = -(int64_t)L + 1 + (int64_t)d;
        y = -(int64_t)L;
    }

    *px = x;
    *py = y;
}

/* Main function: compute a(n) for OEIS A400761. */
seq_t a(idx_t n)
{
    int64_t x, y;
    pos_of(n, &x, &y);

    static const int offs[8][2] = {
        { -1, -1 }, { -1,  0 }, { -1,  1 },
        {  0, -1 },            {  0,  1 },
        {  1, -1 }, {  1,  0 }, {  1,  1 }
    };

    seq_t prod = 1;
    for (int i = 0; i < 8; ++i) {
        int64_t nx = x + offs[i][0];
        int64_t ny = y + offs[i][1];
        prod *= value_at(nx, ny);
    }
    return prod;
}
