/*
 * A308277  Sum of the largest side lengths of all integer-sided triangles
 *          with perimeter n whose sides are square numbers.
 * https://oeis.org/A308277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of the largest side lengths of all integer‑sided *
 *            triangles with perimeter n whose sides are square numbers. *
 *            (Owing to the definition we iterate over all triples of
 *            squares.) * * The routine iterates over all squares a ≤ b ≤ c
 *            with a + b + c = n * and a + b > c, and sums the largest...
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root (floor).  Uses only integer arithmetic. */
static inline idx_t isqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t low = 1, high = (n >> 1) + 1;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t div = n / mid;
        if (mid > div) {
            high = mid - 1;
        } else {
            if (mid + 1 > div) return mid;
            low = mid + 1;
        }
    }
    return low;  /* fallback, though unreachable */
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxroot = isqrt(n);

    for (idx_t i = 1; i <= maxroot; ++i) {
        idx_t a = i * i;
        if (a > n - 2) break;             /* at least two sides of length 1 */

        for (idx_t j = i; j <= maxroot; ++j) {
            idx_t b = j * j;

            /* Ensure there is room for a third side >= 1. */
            if (a + b > n - 1) break;

            idx_t c = n - a - b;
            if (c < b) break;              /* maintain ordering a≤b≤c */

            /* Check whether c is a perfect square. */
            idx_t k = isqrt(c);
            if (k * k != c) continue;

            /* Triangle inequality: a + b > c. */
            if (a + b <= c) continue;

            sum += (seq_t)c;
        }
    }

    return sum;
}
