/*
 * A309316  Euler primary pretenders: a(n) is the smallest odd composite k
 *          such that n^((k+1)/2) == +-n (mod k).
 * https://oeis.org/A309316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Euler primary pretender sequence A309316.
   For a given index n, it finds the smallest odd composite k
   (k ≤ 1729) such that  n^((k+1)/2) ≡ ±n (mod k).
   Implementation: iterative search over odd composite k, testing
   the congruence with modular exponentiation. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    const idx_t MAX_K = 1729;           /* largest composite known for the sequence */
    const idx_t START_K = 9;            /* first odd composite */

    for (idx_t k = START_K; k <= MAX_K; k += 2) {
        /* Skip odd primes: only composites matter */
        if (is_prime(k)) continue;      /* is_prime takes idx_t */

        /* base = n (mod k) – computed using the index type, then cast to seq_t */
        seq_t base_mod = (seq_t)(n % k);

        /* Exponent = (k + 1) / 2  (fits in 32 bits, cast to uint64_t) */
        uint64_t exp = (k + 1) / 2;

        /* Compute n^exp (mod k) */
        seq_t resid = mod_pow(base_mod, exp, (seq_t)k);

        /* Check if resid ≡ ± base_mod (mod k) */
        /* Compute (-base_mod) mod k as an integer to avoid overflow */
        int bm   = (int)base_mod;               /* base_mod < k ≤ 1729  */
        int neg  = (k - bm) % (int)k;           /* 0 ≤ neg < k        */
        seq_t neg_mod = (seq_t)neg;

        if (resid == base_mod || resid == neg_mod)
            return (seq_t)k;
    }

    /* By the mathematical definition, a(k) always exists ≤ 1729.
       This point should never be reached. */
    return (seq_t)0;
}
