/*
 * A379421  a(n) = n + floor(n*r/s) + floor(n*t/s), where r = 5^(1/4), s =
 *          5^(1/2), t = 5^(3/4).
 * https://oeis.org/A379421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/r) + floor(n*r), where r = 5^(1/4)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute floor of the fourth root: largest r such that r^4 ≤ x.  */
static seq_t _floor_fourth_root(seq_t x)
{
    /* 1. Find an upper bound high > x.  */
    seq_t low = 0;
    seq_t high = 1;
    while (high <= x) {
        /* Avoid overflow when shifting high.  */
        if (high > (high << 1))  /* high would overflow next shift */
            break;
        high <<= 1;
    }

    /* 2. Binary search on [low, high).  */
    while (high - low > 1) {
        seq_t mid = low + (high - low) / 2;
        seq_t mid2 = mid * mid;               /* mid^2  */
        /* Guard against overflow in mid^4 by using division:
           mid^4 ≤ x  ⇔  mid^2 ≤ x / mid^2.  */
        if (mid2 > 0 && mid2 <= x / mid2)
            low = mid;   /* mid^4 <= x */
        else
            high = mid;  /* mid^4 > x */
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;            /* n in seq_t */
    /* n^4  */
    seq_t k2 = k * k;              /* k^2   */
    seq_t k4 = k2 * k2;            /* k^4   */

    /* floor(n*r)  = floor( (5*n^4)^(1/4) )  */
    seq_t m_r = _floor_fourth_root(5 * k4);

    /* floor(n/r)  = floor( (n^4 / 5)^(1/4) )  */
    seq_t m_ir = _floor_fourth_root(k4 / 5);

    return k + m_ir + m_r;
}
