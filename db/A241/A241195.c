/*
 * A241195  Denominator of phi(prime(n)-1)/(prime(n)-1), where phi is Euler's
 *          totient function and prime(n) is the n-th prime.
 * https://oeis.org/A241195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of φ(p(n)-1)/(p(n)-1) = (p(n)-1)/gcd(p(n)-1,
 *            φ(p(n)-1))
 * Range:     u8 n <= 54, u16 n <= 6733, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/calc_phi.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset handling – not used for OEIS sequence */
    uint32_t p32 = get_prime(n);          /* p = prime(n) */
    seq_t m = (seq_t)p32 - 1;             /* m = p - 1 */
    seq_t phi_m = calc_phi(m);            /* φ(m) */
    seq_t g = gcd(m, phi_m);              /* g = gcd(m, φ(m)) */
    return m / g;                         /* denominator */
}
