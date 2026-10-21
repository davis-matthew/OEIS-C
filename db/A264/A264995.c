/*
 * A264995  Bijective base-5 reverse: a(0) = 0; for n >= 1, a(n) =
 *          A030104(A132739(n)) * A060904(n).
 * https://oeis.org/A264995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 5^{v5(n)} · rev5(n / 5^{v5(n)}) where rev5(k) is the
 *            integer obtained by reversing the base‑5 digits of k. This
 *            implements the bijective base‑5 reverse true for all n ≥ 0. The
 *            function is self‑inverse and satisfies a(5*n) = 5·a(n)
 * Range:     u8 n <= 127, u16 n <= 9361, u32 n <= 9361, u64 n <= 9361,
 *            u128 n <= 9361, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t temp   = n;      /* value after removing trailing 5's   */
    idx_t pow5   = 0;      /* exponent of 5 (valuation of n)       */

    /* Count how many times n is divisible by 5 */
    while ((temp % 5) == 0)
    {
        temp /= 5;
        ++pow5;
    }

    /* Reverse the base‑5 digits of 'temp' */
    seq_t rev = 0;
    while (temp > 0)
    {
        unsigned int digit = temp % 5;   /* 0 … 4 */
        rev = rev * 5 + digit;
        temp /= 5;
    }

    /* Multiply back by 5^pow5 */
    for (idx_t i = 0; i < pow5; ++i)
        rev *= 5;

    return rev;
}
