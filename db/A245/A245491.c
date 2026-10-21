/*
 * A245491  The least x > 0 such that x < the number of zero digits in the
 *          base-n expansions of the numbers 1 through x.
 * https://oeis.org/A245491
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the least positive integer x such that * zeros_in_base(n,
 *            x) > x * where zeros_in_base(b, m) is the total number of zero *
 *            digits appearing in the base‑b representations of the * integers
 *            1 through m. * * This implementation counts zeros per digit
 *            position using * the standard positional f...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   zeros_in_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/zeros_in_base.h"
/* The types seq_t and idx_t are supplied by the framework. */

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is not defined; return 0, which
     * satisfies the requirement that behaviour below the
     * official offset is irrelevant. */
    if (n < 2)
        return 0;

    seq_t base = (seq_t)n;

    /* Find an upper bound high such that zeros(high) > high. */
    seq_t high = 1;
    while (1)
    {
        seq_t z = zeros_in_base(n, high);
        if (z > high)
            break;
        high <<= 1;            /* multiply by 2; works for arbitrary precision */
    }

    /* Binary search for the smallest x with zeros(x) > x. */
    seq_t lo = 1;
    while (lo < high)
    {
        seq_t mid = lo + ((high - lo) >> 1);
        seq_t zmid = zeros_in_base(n, mid);
        if (zmid > mid)
            high = mid;
        else
            lo = mid + 1;
    }
    return lo;
}
