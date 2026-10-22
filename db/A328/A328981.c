/*
 * A328981  Indicator function of numbers whose binary representation ends in
 *          an even positive number of 0's.
 * https://oeis.org/A328981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the number of trailing zeros in the binary
 *            representation * of n is even and positive, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1.  __builtin_ctzll is defined for
     * unsigned long long and returns the number of trailing zero bits.
     * It is well defined for n > 0. */
    int tz = __builtin_ctzll((unsigned long long)n);

    /* tz is the number of trailing zeros.  We want a positive even count. */
    if (tz > 0 && (tz & 1) == 0)
        return (seq_t)1;
    else
        return (seq_t)0;
}
