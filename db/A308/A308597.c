/*
 * A308597  Number of ways to write n as a*(a+1)/2 + b*(b+1)/2 + 2^c*5^d,
 *          where a,b,c,d are nonnegative integers with a <= b and d <= 1.
 * https://oeis.org/A308597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions of n = a*(a+1)/2 + b*(b+1)/2 +
 *            2^c*5^d with a,b,c,d ∈ {0,1,2,…}, a ≤ b, d ≤ 1. The function
 *            computes this count by brute‑force search over the triangular
 *            numbers t(k)=k(k+1)/2 and testing whether the remainder is a
 *            power of two or five times a power of two
 * Range:     u8 n <= 2642, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_power_of_two(seq_t x)
{
    if (x == 0) return false;
    while (x % (seq_t)2 == 0)
        x /= 2;
    return x == 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, but return 0 for safety */
    seq_t count = 0;

    /* find maximum k such that k*(k+1)/2 ≤ n */
    idx_t kmax = 0;
    while ( ((seq_t)(kmax + 1) * (kmax + 2)) / 2 <= n )
        ++kmax;

    for (idx_t a = 0; a <= kmax; ++a)
    {
        seq_t t_a = (seq_t)a * (a + 1) / 2;
        /* t_a > n cannot happen because a ≤ kmax */
        for (idx_t b = a; b <= kmax; ++b)
        {
            seq_t t_b = (seq_t)b * (b + 1) / 2;
            seq_t sum = t_a + t_b;
            if (sum >= n) break;          /* further b only increase sum */
            seq_t rem = n - sum;

            /* count representations where 2^c*5^d = rem */
            if (is_power_of_two(rem)) ++count;

            if (rem % 5 == 0)
            {
                seq_t q = rem / 5;
                if (is_power_of_two(q)) ++count;
            }
        }
    }
    return count;
}
