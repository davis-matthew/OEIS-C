/*
 * A375036  Perimeter of n when it is considered as a "histonumber" (see
 *          Comments for definition).
 * https://oeis.org/A375036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = sum over decimal digits d of (2+2*d) minus * 2 times the
 *            minimum height of adjacent non‑zero digits. * The digit 0 is
 *            treated as a horizontal segment of unit length * (height 0),
 *            contributing 2 to the perimeter and not overlapping * with
 *            neighbors
 * Range:     u8 n <= 9864, u16 n <= 9864, u32 n <= 9864, u64 n <= 9864,
 *            u128 n <= 9864, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Special case: n = 0 */
    if (n == 0)
        return (seq_t)2;

    /* Extract decimal digits into an array (reversed order). */
    int digits[20];          /* 20 digits suffices for a 64‑bit integer */
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (int)(tmp % 10);
        tmp /= 10;
    }

    /* Compute perimeter using the adjacency rule. */
    seq_t perim = 0;
    int prev = 0;            /* height of previous digit (0 if none) */
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        perim += (seq_t)2 + (seq_t)2 * (seq_t)d;          /* 2 + 2*d */

        if (prev > 0) {
            int m = prev < d ? prev : d;                 /* min(prev,d) */
            perim -= (seq_t)2 * (seq_t)m;                 /* subtract overlap */
        }

        prev = d;
    }

    return perim;
}
