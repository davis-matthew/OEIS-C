/*
 * A333805  Number of odd divisors of n that are < sqrt(n).
 * https://oeis.org/A333805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd divisors of n that are strictly less than
 *            sqrt(n)
 * Range:     u8 n <= 9616, u16 n <= 9616, u32 n <= 9616, u64 n <= 9616,
 *            u128 n <= 9616, big any n
 * Helpers:   iso_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation does not rely on the helper _floor_isqrt_u64; it uses
 * its own integer square‑root routine suitable for 64‑bit unsigned values.
 * The function works for any idx_t >= 1 (idx_t is a 64‑bit unsigned),
 * return type is seq_t (an unsigned integer type).  It counts, for a given
 * n, all odd divisors d of n satisfying d < sqrt(n).  If n is a perfect
 * square the divisor sqrt(n) is not counted.  The routine is O(sqrt(n))
 * in time, which is fast enough for n up to at least 10 000. */

#include <stdbool.h>
#include "../utils/iso_sqrt.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 0; n==0 is undefined but return 0 anyway */
        return 0;

    idx_t sqrt_n = iso_sqrt(n);
    bool is_square = (sqrt_n != 0 && n / sqrt_n == sqrt_n);

    seq_t count = 0;

    /* Count odd divisors less than sqrt_n */
    for (idx_t d = 1; d < sqrt_n; d += 2) {
        if (n % d == 0)
            ++count;
    }

    /* Account for sqrt_n itself if n is not a perfect square and sqrt_n is odd */
    if (!is_square && (sqrt_n % 2 == 1) && (n % sqrt_n == 0))
        ++count;

    return count;
}
