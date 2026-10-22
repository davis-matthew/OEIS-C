/*
 * A333848  a(n) gives the sum of the odd numbers of the smallest nonnegative
 *          reduced residue system modulo 2*n + 1, for n >= 0.
 * https://oeis.org/A333848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k odd, 1 ≤ k < 2*n+1, gcd(k, 2*n+1)=1} k
 * Range:     u8 n <= 17, u16 n <= 259, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sum = 0;
    idx_t N = 2 * n + 1;                 /* modulus, always odd */
    for (idx_t k = 1; k < N; k += 2)     /* iterate odd residues */
    {
        if (gcd((seq_t)k, (seq_t)N) == 1)
            sum += k;
    }
    return sum;
}
