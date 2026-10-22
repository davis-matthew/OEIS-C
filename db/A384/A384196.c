/*
 * A384196  Consecutive states of the linear congruential pseudo-random number
 *          generator 20403*s mod 2^15 when started at s=1.
 * https://oeis.org/A384196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20403 * a(n-1) mod 2^15 with a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= (idx_t)1)
        return (seq_t)1;

    /* constants fit in unsigned long long */
    const unsigned long long mul = 20403ull;
    const unsigned long long mod = 32768ull;   /* 2^15 */

    seq_t result = (seq_t)1;
    for (idx_t i = (idx_t)1; i < n; ++i)
    {
        result = (seq_t)(((unsigned long long)result * mul) % mod);
    }
    return result;
}
