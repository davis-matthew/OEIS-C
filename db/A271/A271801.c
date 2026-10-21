/*
 * A271801  Smallest composite k such that k divides (n^(k-1)-1)/(n-1), n > 1.
 * https://oeis.org/A271801
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite k such that * k divides
 *            (n^(k-1)-1)/(n-1) and gcd(k,n-1)=1, n > 1
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime, mod_pow_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"
#include "../utils/mod_pow_custom.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;      /* undefined for n < 2 */
    seq_t base    = (seq_t)n;
    seq_t n_minus = (seq_t)(n - 1);

    for (idx_t k = 4; ; ++k) {
        /* Skip primes; we need a composite k. */
        if (is_prime(k))
            continue;

        /* Ensure coprime with n-1. */
        if (gcd((seq_t)k, n_minus) != (seq_t)1)
            continue;

        /* Check pseudoprime condition: n^(k-1) ≡ 1 (mod k). */
        seq_t r = mod_pow_custom(base, (uint64_t)(k - 1), (seq_t)k);
        if (r == (seq_t)1)
            return (seq_t)k;
    }
}
