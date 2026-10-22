/*
 * A318840  a(n) = phi(n) - Product_{primes p dividing n} gcd(p-1, n-1).
 * https://oeis.org/A318840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n) - prod_{p | n} gcd(p-1, n-1)
 * Range:     u8 n <= 257, u16 n <= 9837, u32 n <= 9837, u64 n <= 9837,
 *            u128 n <= 9837, big any n
 * Helpers:   factorize, gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorize.h"
#include "../utils/gcd.h"
#include "../utils/phi.h"

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* Compute phi(n) */
    seq_t phi_val = phi(n);

    /* Edge case n == 1: product is 1 by convention */
    seq_t prod = 1;

    /* Factor n to get distinct prime divisors */
    uint64_t primes[64];
    int exps[64];
    size_t cnt = factorize(n, primes, exps);

    /* Precompute n-1 once */
    seq_t n_minus_one = (seq_t)(n - 1);

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p_minus_one = (seq_t)(primes[i] - 1);
        seq_t g = gcd(p_minus_one, n_minus_one);
        prod *= g;
    }

    return phi_val - prod;
}
