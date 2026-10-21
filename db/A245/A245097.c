/*
 * A245097  Number of primes greater than n and not greater than the smallest
 *          pseudoprime to base n.
 * https://oeis.org/A245097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = π(A007535(n)) - π(n)
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, gcd, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A007535(n) is the smallest composite k such that gcd(n,k)=1
   and n^(k-1) ≡ 1 (mod k).  The function uses the utility libraries
   to test primality, compute modular exponentiation and prime counts. */

#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* Find A007535(n): the smallest base‑n pseudoprime */
    idx_t k = n + 1;
    for (;;) {
        /* Skip primes */
        if (is_prime(k)) {
            ++k;
            continue;
        }
        /* Must be coprime to the base */
        if (gcd((seq_t)n, (seq_t)k) != (seq_t)1) {
            ++k;
            continue;
        }
        /* Test Fermat congruence */
        seq_t rem = mod_pow((seq_t)n, (uint64_t)(k - 1), (seq_t)k);
        if (rem == (seq_t)1)
            break;
        ++k;
    }

    /* Compute π(A007535(n)) and π(n) */
    seq_t pi_k = compute_pi(k);
    seq_t pi_n = compute_pi(n);

    return pi_k - pi_n;
}
