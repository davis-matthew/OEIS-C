/*
 * A284406  Odd numbers k such that lambda(k) < phi(k) and gcd(lambda(k), k-1)
 *          = gcd(phi(k), k-1).
 * https://oeis.org/A284406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Implementation of OEIS sequence A284406:
 * a(n) is the n-th odd integer k such that λ(k) < φ(k) and
 * gcd(λ(k), k-1) == gcd(φ(k), k-1).
 * λ(k) is Carmichael's function (for odd k: lcm of p^{e-1}(p-1))
 * φ(k) is Euler's totient.
 * The function computes terms by a simple linear search over odd
 * integers, using the provided gcd helper. No dynamic allocation
 * is required, so the code compiles as both C11 and C++17.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static bool test_k(idx_t k)
{
    idx_t n = k;
    seq_t phi = 1;
    seq_t lcm = 1;

    /* factorise n and accumulate φ and λ components */
    for (idx_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                exp++;
            }
            /* compute φ(p^exp) = p^{exp-1}*(p-1) */
            seq_t pow = 1;
            for (idx_t i = 1; i < exp; i++)
                pow *= (seq_t)p;
            seq_t term = ((seq_t)(p - 1)) * pow;
            phi *= term;
            seq_t g = gcd(lcm, term);
            lcm = (lcm / g) * term;
        }
    }
    if (n > 1) {                // remaining prime factor
        seq_t term = (seq_t)(n - 1);
        phi *= term;
        seq_t g = gcd(lcm, term);
        lcm = (lcm / g) * term;
    }

    /* λ(k) = lcm, φ(k) = phi */
    if (!(lcm < phi))
        return false;
    seq_t g1 = gcd(lcm, (seq_t)(k - 1));
    seq_t g2 = gcd(phi,  (seq_t)(k - 1));
    return g1 == g2;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          // undefined for n=0
    idx_t count = 0;
    idx_t k = 1;                  // start from 1, then odd numbers
    while (count < n) {
        k += 2;                    // next odd integer
        if (test_k(k))
            count++;
    }
    return (seq_t)k;
}
