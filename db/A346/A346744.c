/*
 * A346744  The number of congruences k^(n-k) + (n-k)^k == 0 (mod n) with 0 <
 *          k < n.
 * https://oeis.org/A346744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{1 ≤ k ≤ n-1 : k^(n-k) + (n-k)^k ≡ 0 (mod n)}
 * Range:     u8 n <= 970, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t count = 0;
    seq_t mod   = (seq_t)n;

    for (idx_t k = 1; k < n; ++k)
    {
        idx_t nk = n - k;

        seq_t t1 = mod_pow((seq_t)k, nk, mod);
        seq_t t2 = mod_pow((seq_t)nk, k, mod);

        if ( ((t1 + t2) % mod) == (seq_t)0 )
            ++count;
    }

    return count;
}
