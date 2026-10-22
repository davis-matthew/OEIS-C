/*
 * A341409  a(n) = (Sum_{k=1..3} k^n) mod n.
 * https://oeis.org/A341409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1^n + 2^n + 3^n) mod n
 * Range:     u8 n <= 271, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* For n=1 the modulus would be zero, return the known value directly. */
    if (n == 1)
        return (seq_t)0;

    seq_t mod = (seq_t)n;               /* ensure correct type everywhere */
    seq_t sum = mod_pow((seq_t)1, (uint64_t)n, mod);
    sum += mod_pow((seq_t)2, (uint64_t)n, mod);
    sum += mod_pow((seq_t)3, (uint64_t)n, mod);
    return sum % mod;
}
