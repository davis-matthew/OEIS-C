/*
 * A387644  Complement of A045944.
 * https://oeis.org/A387644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + m if n > m*(3*m+1) a(n) = n + m - 1 otherwise where m
 *            = floor(sqrt(n/3)) (OEIS A387644)
 * Range:     u8 n <= 247, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of x : floor(sqrt(x)) */
static idx_t int_sqrt(idx_t x)
{
    idx_t low = 0, high = x, mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid == 0) {
            res = 0;
            low = 1;
            continue;
        }
        if (mid <= x / mid) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* safe guard */
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* n is 1‑based, offset 1 */
    idx_t m = int_sqrt(n / 3);
    idx_t bound = m * (3 * m + 1);
    if (n > bound)
        return (seq_t)n + (seq_t)m;
    else
        return (seq_t)n + (seq_t)m - (seq_t)1;
}
