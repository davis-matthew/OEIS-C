/*
 * A364847  Number of conjugacy classes in the group SL(2, Z_n), up to
 *          conjugacy in GL(2, Z_n).
 * https://oeis.org/A364847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} d * A060594(n/d)
 * Range:     u8 n <= 59, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A060594(m) is the number of solutions of x^2 = 1 (mod m) */

#include <stdint.h>
#include <stdbool.h>

/* Helper: number of solutions to x^2 ≡ 1 (mod m) */
static seq_t A060594_value(idx_t m)
{
    /* count exponent of 2 */
    idx_t exp2 = 0;
    while ((m & 1U) == 0U)
    {
        m >>= 1U;
        exp2++;
    }

    /* after removing powers of 2, m is odd */
    idx_t cnt_odd = 0;
    idx_t temp = m;
    for (idx_t p = 3U; p * p <= temp; p += 2U)
    {
        if (temp % p == 0U)
        {
            cnt_odd++;
            while (temp % p == 0U)
                temp /= p;
        }
    }
    if (temp > 1U)   /* remaining prime factor */
        cnt_odd++;

    seq_t res = 1;
    for (idx_t i = 0; i < cnt_odd; ++i)
        res <<= 1U;   /* multiply by 2 for each distinct odd prime */

    /* multiply by the factor contributed by powers of 2 */
    if (exp2 >= 3U)
        res *= 4U;
    else if (exp2 == 2U)
        res *= 2U;
    /* exp2 == 0 or 1 -> multiply by 1 (nothing to do) */

    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over divisors d using i <= n/i to avoid overflow */
    for (idx_t i = 1U; i <= n / i; ++i)
    {
        if (n % i == 0U)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            sum += (seq_t)d1 * A060594_value(d2);
            if (d1 != d2)
                sum += (seq_t)d2 * A060594_value(d1);
        }
    }
    return sum;
}
