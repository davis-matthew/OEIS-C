/*
 * A258149  Triangle of the absolute difference of the two legs (catheti) of
 *          primitive Pythagorean triangles.
 * https://oeis.org/A258149
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = a(row, col) where the triangle a(row, col) is filled
 *            row-wise. Let pos = n - 1 (1-based position in the flattened
 *            triangle). Then the row r is the smallest integer such that
 *            r*(r-1)/2 >= pos. The column c is pos - (r-1)*(r-2)/2. The
 *            triangle entry is 0 if gcd(r, c) != 1 or r and c have sam...
 * Range:     u8 n <= 45, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* offset is 2 */

    idx_t pos = n - 1;                  /* 1‑based position in the flattened triangle */

    /* Find the minimal row r such that r*(r-1)/2 >= pos */
    idx_t low  = 2;
    idx_t high = pos + 2;                /* high is guaranteed to satisfy the inequality */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t_mid = (unsigned __int128)mid * (mid - 1) / 2;
        if (t_mid >= pos) high = mid;
        else              low  = mid + 1;
    }
    idx_t r = low;

    /* Column index c */
    unsigned __int128 t_prev128 = (unsigned __int128)(r - 1) * (r - 2) / 2;
    idx_t t_prev = (idx_t)t_prev128;            /* number of elements before row r */
    idx_t c = pos - t_prev;                     /* 1 <= c <= r-1 */

    /* Primitive condition: coprime and opposite parity */
    if (((r ^ c) & 1) == 0) return 0;          /* same parity → not primitive */
    if (gcd((seq_t)r, (seq_t)c) != 1) return 0;/* not coprime -> not primitive */

    /* Compute |r^2 - c^2 - 2*r*c| without overflow */
    seq_t t1 = (seq_t)r * r;                    /* r^2 */
    seq_t t2 = (seq_t)c * c;                    /* c^2 */
    seq_t t3 = (seq_t)2 * r * c;                /* 2*r*c */
    seq_t sum = t2 + t3;                        /* c^2 + 2*r*c */

    return (t1 >= sum) ? (t1 - sum) : (sum - t1);
}
