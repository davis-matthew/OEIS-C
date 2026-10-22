/*
 * A366785  Greatest common divisor of the number of prime factors (counted
 *          with multiplicity, A001222) and the greatest prime index (A061395)
 *          of n.
 * https://oeis.org/A366785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A001222(n), A061395(n))
 * Range:     u8 n <= 3328, u16 n <= 9640, u32 n <= 9640, u64 n <= 9640,
 *            u128 n <= 9640, big any n
 * Helpers:   count_prime_factors, gcd, largest_prime_factor, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A001222(n) - number of prime factors of n (with multiplicity) */
/* A061395(n) - index of the greatest prime factor of n (1‑based) */

#include "../utils/gcd.h"
#include "../utils/count_prime_factors.h"
#include "../utils/largest_prime_factor.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        /* By definition A366785(1) = 0 */
        return (seq_t)0;
    }

    /* Number of prime factors (with multiplicity) */
    seq_t omega = count_prime_factors((seq_t)n);

    /* Largest prime factor of n */
    uint64_t lp = largest_prime_factor(n);

    /* Index of that prime (1‑based) */
    seq_t idx = prime_pi(lp);

    /* GCD of the two values */
    return gcd(omega, idx);
}
