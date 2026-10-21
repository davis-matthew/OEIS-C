/*
 * A295819  Number of nonnegative solutions to (x,y) = 1 and x^2 + y^2 = n.
 * https://oeis.org/A295819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer solutions (x,y) with
 *            gcd(x,y) = 1 and x^2 + y^2 = n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Integer square root for uint64_t values using binary search. */
static idx_t isqrt_u64(idx_t n) {
    if (n == 0) return 0;
    idx_t low = 0, high = n, mid;
    while (low <= high) {
        mid = (low + high) >> 1;
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t count = 0;
    idx_t limit = isqrt_u64(n);

    for (idx_t x = 0; x <= limit; ++x) {
        idx_t y2 = n - x * x;
        idx_t y = isqrt_u64(y2);
        if (y * y != y2) continue;          /* not a perfect square */

        /* Handle the special zero cases, keeping only (0,1) and (1,0). */
        if (x == 0 || y == 0) {
            if ((x == 0 && y == 1) || (y == 0 && x == 1))
                count += 1;
            continue;
        }

        /* Now x > 0 and y > 0.  Only count each unordered pair once. */
        if (y < x) continue;                /* skip symmetric duplicate */

        if (gcd((seq_t)x, (seq_t)y) == (seq_t)1) {
            if (x == y)
                count += 1;                 /* x == y gives one ordered pair */
            else
                count += 2;                 /* (x,y) and (y,x) distinct */
        }
    }
    return count;
}
