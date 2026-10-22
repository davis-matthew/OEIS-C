/*
 * A327637  a(n) is the number of integers j such that 1 <= j <= n and
 *          gcd(n,j) is a triangular number.
 * https://oeis.org/A327637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A010054(n/d) * phi(d) ( A010054(k) = 1 if k is
 *            a triangular number, otherwise 0 )
 * Range:     u8 n <= 31, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* local integer square root for small seq_t values */
static inline seq_t int_sqrt_seq(seq_t v)
{
    seq_t r = 0;
    while ((r + 1) * (r + 1) <= v)
        r++;
    return r;
}

/* test whether x is triangular: 8*x + 1 is a perfect square */
static inline bool is_triangular(seq_t x)
{
    seq_t v = 8 * x + 1;
    seq_t r = int_sqrt_seq(v);
    return r * r == v;
}

/* Euler's totient function for small integers (idx_t is unsigned) */
static inline seq_t phi_uint64(idx_t m)
{
    if (m <= 1)
        return (seq_t)m;   /* phi(1) = 1, phi(0) = 0 */

    seq_t result = (seq_t)m;
    idx_t temp   = m;

    for (idx_t p = 2; (seq_t)p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            while (temp % p == 0)
                temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1)
        result -= result / temp;
    return result;
}

/* main sequence function */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* integer sqrt of n for divisor enumeration */
    idx_t sqrt_n = 0;
    while ((sqrt_n + 1) * (sqrt_n + 1) <= n)
        sqrt_n++;

    for (idx_t i = 1; i <= sqrt_n; ++i)
    {
        if (n % i != 0)
            continue;

        idx_t d1 = i;
        seq_t g1 = (seq_t)n / d1;
        if (is_triangular(g1))
            sum += phi_uint64(d1);

        idx_t d2 = n / i;
        if (d2 != d1)
        {
            seq_t g2 = (seq_t)n / d2;
            if (is_triangular(g2))
                sum += phi_uint64(d2);
        }
    }

    return sum;
}
