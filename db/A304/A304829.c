/*
 * A304829  a(n) = 4024*7^n/147 - 256/3 (n >= 2).
 * https://oeis.org/A304829
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8/3) * (503*7^(n-2) - 32) for n ≥ 2
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The expression is integer‐valued because (503*7^(n-2)-32) is always a multiple of 3. */

seq_t a(idx_t n)
{
    if (n < 2)          /* sequence offset is 2 – undefined below */
        return 0;

    /* compute 7^(n-2) */
    seq_t pow = 1;
    for (idx_t i = 0; i < n - 2; ++i)
        pow *= (seq_t)7;

    /* compute ((503*pow - 32) / 3) * 8 */
    seq_t tmp = 503 * pow;
    tmp -= 32;
    tmp /= 3;           /* exact division */
    return tmp * 8;
}
