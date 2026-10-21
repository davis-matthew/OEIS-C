/*
 * A245470  Smallest multiple of n such that, when expressed in binary, in the
 *          string of bits the binary representation of n occurs after the n-1
 *          most significant bits.
 * https://oeis.org/A245470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (2^{n-2} + mod(-(2^{n-2}), n')) * 2^d + n where d =
 *            bitlen(n) (number of bits in n) and n' = n / 2^valuation(n,2) =
 *            odd part of n (the mod() function returns a non‑negative result)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Special case n = 1: a(1) = 1 */
    if (n == (idx_t)1) return (seq_t)1;

    /* Compute bit length d of n */
    unsigned int d = 0;
    idx_t tmp = n;
    while (tmp) { d++; tmp >>= 1; }

    /* Compute valuation of n at 2: number of trailing zero bits */
    unsigned int tz = __builtin_ctzll(n);

    /* Odd part n' = n >> tz */
    seq_t n_prime = ((seq_t)n) >> tz;   /* tz <= 63 for n>=1 */

    /* Compute 2^(n-2) */
    seq_t pow_n2 = pow2((idx_t)n - 2);

    /* Compute (-pow_n2) mod n_prime, i.e. (n_prime - (pow_n2 % n_prime)) % n_prime */
    seq_t mod_val = pow_n2 % n_prime;
    seq_t r = (n_prime - mod_val) % n_prime;

    /* Combine and shift */
    seq_t m   = pow_n2 + r;
    seq_t res = (m << d) + (seq_t)n;

    return res;
}
