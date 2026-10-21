/*
 * A244342  a(n) = phi(n)*h(n) where phi() is the Euler totient function,
 *          A000010, and h() is A092089.
 * https://oeis.org/A244342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{k=1..n, gcd(k,n)=1} gcd(k^2-1, n) (OEIS A244342)
 *            Implementation uses a direct summation over reduced residues
 * Range:     u8 n <= 17, u16 n <= 435, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; value for 0 is irrelevant */

    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        if (gcd((seq_t)k, (seq_t)n) == 1)
        {
            seq_t val = (seq_t)k * (seq_t)k - (seq_t)1;
            seq_t g   = gcd(val, (seq_t)n);
            total += g;
        }
    }
    return total;
}
