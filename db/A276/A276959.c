/*
 * A276959  Sum of squares of digits in all divisors of n.
 * https://oeis.org/A276959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{d|n} digit_square_sum(d)
 * Range:     u8 n <= 35, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = sum of the squares of the decimal digits of all divisors of n.
   The implementation uses only integer arithmetic and is efficient enough
   for n up to 10 000 (about 100 operations per call). */

#include <stdint.h>

/* Integer square root for idx_t (non‑negative). */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    idx_t mid;
    while (low <= high)
    {
        mid = (low + high) >> 1;
        if (mid > 0 && mid > n / mid)  /* mid*mid > n, avoid overflow */
            high = mid - 1;
        else
            low = mid + 1;
    }
    return high;   /* high*high <= n < (high+1)*(high+1) */
}

/* Sum of squares of decimal digits of a uint64_t. */
static seq_t digit_square_sum_uint64(uint64_t x)
{
    seq_t sum = 0;
    while (x > 0)
    {
        seq_t d = (seq_t)(x % 10);
        sum += d * d;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t lim = isqrt_u64(n);
    for (idx_t i = 1; i <= lim; ++i)
    {
        if (n % i == 0)
        {
            sum += digit_square_sum_uint64((uint64_t)i);
            idx_t d = n / i;
            if (d != i)
                sum += digit_square_sum_uint64(d);
        }
    }
    return sum;
}
