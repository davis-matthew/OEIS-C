/*
 * A246763  Catalan(n)^2 mod prime(n).
 * https://oeis.org/A246763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n)^2 mod prime(n) (prime(n) is the n-th prime)
 * Range:     u8 n <= 54, u16 n <= 5205, u32 n <= 7762, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Catalan numbers are computed modulo prime(n) using the identity  
 *   Catalan(n) = ∏_{i=2}^{n} (n+i) * inv(i)  (mod prime(n))
 * where inv(i) = i^{prime(n)-2} (mod prime(n)) by Fermat's little theorem. 
 * The result is then squared modulo the same prime. */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* obtain the n-th prime (fits in 32 bits for n up to 10000) */
    uint64_t p64 = get_prime(n);
    seq_t mod = (seq_t)p64;

    /* Catalan number modulo mod */
    seq_t catalan = 1;
    seq_t nn = (seq_t)n;

    for (idx_t i = 2; i <= n; ++i) {
        /* multiply by (n + i) modulo mod */
        catalan = (catalan * (nn + (seq_t)i)) % mod;

        /* multiply by the inverse of i modulo mod */
        seq_t inv = mod_pow((seq_t)i, p64 - 2, mod);
        catalan = (catalan * inv) % mod;
    }

    /* return Catalan(n)^2 mod prime(n) */
    return (catalan * catalan) % mod;
}
