/*
 * A353677  a(n) = 1 if n is an odd number of the form p^j * q^k, with p and q
 *          primes and gcd(phi(p^j), phi(q^k)) = 2, otherwise 0.
 * https://oeis.org/A353677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = p^j * q^k with p,q primes, j,k>0, n odd,
 *            gcd(phi(p^j),phi(q^k)) = 2; otherwise 0
 * Range:     u8 n <= 6893, u16 n <= 6893, u32 n <= 6893, u64 n <= 6893,
 *            u128 n <= 6893, big any n
 * Helpers:   factor_u64, gcd, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_int.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* 1-indexed: a(1) = 0 */
    if (n <= 1) return (seq_t)0;

    /* Even numbers cannot satisfy the requirement */
    if ((n & 1) == 0) return (seq_t)0;

    uint64_t num = (uint64_t)n;

    /* Factorize n; only two distinct primes with positive exponents are allowed */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64(num, primes, exps, &cnt);

    if (cnt != 2) return (seq_t)0;

    uint64_t p = primes[0];
    uint64_t q = primes[1];
    uint64_t j = exps[0];
    uint64_t k = exps[1];

    /* Compute phi(p^j) = p^(j-1) * (p-1) and phi(q^k) similarly */
    seq_t phi_p = (seq_t)(p - 1) * pow_int((seq_t)p, (idx_t)(j - 1));
    seq_t phi_q = (seq_t)(q - 1) * pow_int((seq_t)q, (idx_t)(k - 1));

    seq_t g = gcd(phi_p, phi_q);
    return (g == (seq_t)2) ? (seq_t)1 : (seq_t)0;
}
