/*
 * A248962  a(n) = PrimePi(log(prime(n))*n).
 * https://oeis.org/A248962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = PrimePi(floor(log(prime(n))*n))
 * Range:     u8 n <= 223, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, but guard against 0 */
    uint32_t p = get_prime(n);     /* prime(n) */
    double prod = log((double)p) * (double)n;  /* log(p)*n */
    idx_t val = (idx_t)(prod + 1e-12);          /* floor */
    return compute_pi(val);                      /* number of primes ≤ val */
}
