/*
 * A379596  a(n) is the least positive integer k for which k^2 + (k + n)^2 is
 *          a square.
 * https://oeis.org/A379596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the least positive integer k for which * k^2 + (k + n)^2
 *            is a square
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 9923, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root (floor) for arbitrary seq_t.
 * This routine is generic and uses only arithmetic operations
 * allowed by the problem statement. */
static seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x;
    seq_t ans = 0;

    while (lo <= hi)
    {
        seq_t mid = lo + ((hi - lo) >> 1);
        if (mid != 0 && mid > x / mid)   /* mid*mid would overflow or be > x */
        {
            hi = mid - 1;
            continue;
        }
        /* Now mid*mid <= x */
        ans = mid;
        lo = mid + 1;
    }
    return ans;
}

/* OEIS A379596 */
seq_t a(idx_t n_input)
{
    if (n_input == 0)           /* out of range; not used by the build */
        return 0;

    seq_t n = (seq_t)n_input;
    seq_t m = n + 2;            /* first m with same parity as n and m > n */

    /* Pre‑compute n^2 once. */
    seq_t n_sq = n * n;

    for (;; m += 2)            /* increment m by 2 to keep parity */
    {
        seq_t m_sq = m * m;
        seq_t sum = n_sq + m_sq;      /* n^2 + m^2 (even because n and m share parity) */
        seq_t half = sum / 2;          /* exact integer division */
        seq_t t = floor_sqrt(half);    /* integer square root of half */

        if (t * t == half)            /* found a square */
            return (m - n) / 2;       /* k = (m - n)/2 */
    }
}
