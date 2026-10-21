/*
 * A276269  a(n) = n + the largest (if n is even), or the smallest (if n is
 *          odd) prime factor of n if n > 1; and a(1)=1.
 * https://oeis.org/A276269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n + (n even ? largest prime factor of n : smallest prime
 *            factor of n), a(1)=1
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/largest_prime_factor.h"
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    seq_t result = (seq_t)n;
    if ((n & 1ULL) == 0)   /* even */
        result += (seq_t)largest_prime_factor((uint64_t)n);
    else                   /* odd */
        result += (seq_t)_smallest_prime_factor((uint64_t)n);
    return result;
}
