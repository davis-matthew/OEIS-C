/*
 * A360264  Sum of mass(k/n) for all k, 1 <= k <= n, that are relatively prime
 *          to n.
 * https://oeis.org/A360264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{1 ≤ k ≤ n, gcd(k,n)=1} mass(k/n),
 * where mass(k/n) is the sum of the quotients in the Euclidean
 * algorithm that computes gcd(k,n).
 */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        if (gcd((seq_t)k, (seq_t)n) == 1)
        {
            seq_t a_val = (seq_t)k;
            seq_t b_val = (seq_t)n;
            seq_t mass  = 0;
            while (b_val != 0)
            {
                seq_t q   = a_val / b_val;
                mass += q;
                seq_t rem = a_val % b_val;
                a_val = b_val;
                b_val = rem;
            }
            total += mass;
        }
    }
    return total;
}
