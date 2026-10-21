/*
 * A261905  Numbers m with the property that there is no number k in the range
 *          ceiling(sqrt(3m)) <= k <= floor(sqrt(4m)) with k == m (mod 2).
 * https://oeis.org/A261905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Property: m is in the sequence iff for every integer k in  
   ceil(sqrt(3m)) ≤ k ≤ floor(sqrt(4m)) we have k ≢ m (mod 2).  
   Equivalently the interval is empty, or it contains a single integer
   whose parity differs from that of m. */

/* Integer square root: floor(sqrt(x)) for an unsigned seq_t. */
static seq_t isqrt(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low < high)
    {
        seq_t mid = (low + high + 1) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t m = 1;
    while (count < n)
    {
        /* Compute low = ceil(sqrt(3m)) and high = floor(sqrt(4m)). */
        seq_t three_m = m * 3;
        seq_t four_m = m * 4;
        seq_t sqrt3_floor = isqrt(three_m);
        seq_t sqrt4_floor = isqrt(four_m);

        seq_t low = sqrt3_floor;
        if (low * low < three_m)   /* not a perfect square */
            ++low;                  /* ceil */
        seq_t high = sqrt4_floor;

        bool ok;
        if (high < low)          /* empty interval */
            ok = true;
        else if (high == low)    /* single integer */
            ok = ((high & 1) != (m & 1));
        else                     /* interval longer than 1: impossible */
            ok = false;

        if (ok) {
            ++count;
            if (count == n)
                return m;
        }
        ++m;
    }
    return 0;   /* should never happen for n ≥ 1 */
}
