/*
 * A307109  Number of integer solutions to x^2 + x*y + y^2 <= n excluding
 *          (0,0), divided by 2.
 * https://oeis.org/A307109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A307109
 * Formula: a(n) = ½ · |{(x,y)∈ℤ² : x≠0, y≠0, x² + x·y + y² ≤ n}|
 *
 * The implementation counts all ordered pairs (x,y) with non‑zero
 * coordinates that satisfy the quadratic inequality, then divides
 * the total by 2.  The division is always integral because
 * (x,y) ↦ (−x,−y) gives a distinct partner for every counted
 * pair.
 *
 * The counting is done by enumerating |x| up to the largest |x|
 * for which the inequality can hold:
 *
 *   3·x² ≤ 4·n       →   |x| ≤ ⌊√(4n/3)⌋
 *
 * For each such x we test all y in the same range;  the double
 * loop size is bounded by a few tens of thousands for n≤10000,
 * which meets the performance goal.
 *
 * All arithmetic that may overflow 64 bits is performed in
 * __int128.  The function returns its result cast to seq_t.
 */

#include <stdbool.h>

/* Integer square‑root of a __int128 value, returned as an idx_t.
   Uses a binary search with __int128 intermediate multiplication. */
static idx_t isqrt128(__int128 v)
{
    idx_t low = 0, high = (idx_t)-1;  /* maximum idx_t value */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == v) return mid;
        if (sq < v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* floor(sqrt(v)) */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset is 1; values below no‑ops */

    /* Compute the bound for |x|, |y| :  ⌊√(4n/3)⌋. */
    __int128 lim_val = (__int128)n * 4 / 3;
    idx_t   lim_idx = isqrt128(lim_val);
    long long lim = (long long)lim_idx;          /* safe: lim fits in long long  (≈5×10⁹ for n≈2⁶⁴) */

    unsigned long long total = 0;   /* number of ordered pairs (x,y) with x≠0,y≠0 */

    for (long long x = -lim; x <= lim; ++x) {
        if (x == 0) continue;
        for (long long y = -lim; y <= lim; ++y) {
            if (y == 0) continue;
            __int128 q = (__int128)x * x + (__int128)x * y + (__int128)y * y;
            if (q <= (__int128)n)
                ++total;
        }
    }

    return (seq_t)(total / 2);
}
