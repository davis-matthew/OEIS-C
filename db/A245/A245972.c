/*
 * A245972  Tower of 5s mod n.
 * https://oeis.org/A245972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^a(phi(n)) mod n, where phi is Euler’s totient (a(1) =
 *            0)
 * Range:     u8 n <= 279, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   modpow_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/modpow_local.h"

/* compute the Euler totient of n; n fits in idx_t */
static seq_t phi_val(idx_t n)
{
    if (n == 0) return 0;   /* unused, but harmless */
    idx_t m = n;
    idx_t result = n;

    /* handle factor 2 */
    if ((m & 1) == 0) {            /* m even */
        while ((m & 1) == 0) m >>= 1;
        result -= result >> 1;     /* result -= result / 2 */
    }

    /* handle odd primes */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }

    /* if a prime factor greater than sqrt remained */
    if (m > 1) result -= result / m;
    return (seq_t)result;
}

/* recursive computation of a(n) */
static seq_t compute_a(idx_t n)
{
    if (n == 1) return (seq_t)0;          /* a(1) = 0 */
    idx_t phi_n = (idx_t)phi_val(n);
    seq_t exp = compute_a(phi_n);         /* a(phi(n)) */
    return modpow_local((seq_t)5, (uint64_t)exp, (seq_t)n);
}

/* public entry point required by the test harness */
seq_t a(idx_t n)
{
    return compute_a(n);
}
