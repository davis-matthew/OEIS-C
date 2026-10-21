/*
 * A242848  Number of unimodular roots of the equation z^n + z^k - 1 for all 1
 *          <= k <= n-1.
 * https://oeis.org/A242848
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n-1; 6 divides (n/gcd(n,k)+k/gcd(n,k))}
 *            2*gcd(n,k)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k)
    {
        seq_t g = gcd((seq_t)n, (seq_t)k);          /* g = gcd(n,k) */
        seq_t t = ((seq_t)(n / g)) + ((seq_t)(k / g)); /* t = n/g + k/g */
        if ((t % 6) == 0)
            sum += (g << 1);                         /* add 2*g */
    }
    return sum;
}
