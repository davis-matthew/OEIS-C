/*
 * A338732  Generating function Sum_{n >= 0} a(n)*x^n = Sum_{k>=1}
 *          x^(k*(3*k+1))/(1-x^k).
 * https://oeis.org/A338732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive integers k such that k divides n and n
 *            >= k*(3*k+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; ; ++k)
    {
        /* Stop when k*(3*k+1) exceeds n */
        __int128 prod = (__int128)k * (3 * k + 1);
        if (prod > n) break;
        if (n % k == 0) ++count;
    }
    return count;
}
