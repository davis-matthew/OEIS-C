/*
 * A252424  Numbers k such that sum of odd divisors of k equals sum of squares
 *          of primes dividing k.
 * https://oeis.org/A252424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth smallest number of the form 18*2^k or 234*2^k,
 *            k>=0
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 53, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1u) return (seq_t)18;
    if (n == 2u) return (seq_t)36;
    if (n == 3u) return (seq_t)72;
    if (n == 4u) return (seq_t)144;
    if (n == 5u) return (seq_t)234;

    idx_t k = n - 6u;          /* since first 5 terms handled separately   */
    if ((k & 1u) == 0u) {      /* even k  -> 18*2^(k/2+4) = 288*2^(k/2)     */
        seq_t pw = pow2(k / 2u);
        return (seq_t)288 * pw;
    } else {                   /* odd k   -> 234*2^((k-1)/2+1) = 468*2^((k-1)/2) */
        seq_t pw = pow2((k - 1u) / 2u);
        return (seq_t)468 * pw;
    }
}
