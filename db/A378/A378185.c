/*
 * A378185  a(n) = n + floor(n*r/s) + floor(n*r/t), where r=2^(1/4),
 *          s=2^(1/2), t=2^(3/4).
 * https://oeis.org/A378185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*w) + floor(n/w) with w = 2^(1/4)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the integer floor of the fourth root of x */
static seq_t _floor_fourth_root(seq_t x)
{
    seq_t low  = 0;
    seq_t high = 1;

    /* Increase high until high^4 > x */
    while (1)
    {
        seq_t h2 = high * high;
        seq_t h4 = h2 * h2;
        if (h4 > x) break;
        high = high * 2;
    }

    /* Binary search on [low, high] */
    while (low < high)
    {
        seq_t mid = (low + high + 1) / 2;
        seq_t m2   = mid * mid;
        seq_t m4   = m2 * m2;
        if (m4 <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* OEIS A378185 sequence */
seq_t a(idx_t n)
{
    /* Convert index to seq_t and compute n^4 */
    seq_t n_t = (seq_t)n;
    seq_t n2  = n_t * n_t;      /* n^2 */
    seq_t n4  = n2 * n2;        /* n^4 */

    /* floor(n*w)  = floor( (n^4 * 2)^(1/4) ) */
    seq_t term1 = _floor_fourth_root(n4 * 2);

    /* floor(n/w)  = floor( (n^4 / 2)^(1/4) ) */
    seq_t term2 = _floor_fourth_root(n4 / 2);

    return n_t + term1 + term2;
}
