/*
 * A354946  a(n) = gcd(A001923(n),n^n).
 * https://oeis.org/A354946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A001923(n), n^n) where A001923(n) = Σ_{k=1..n} k^k
 * Range:     u8 n <= 35, u16 n <= 4387, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factorize, mod_pow, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the required value by
   determining, for each prime factor p of n,
   the exponent of p in the sum S(n) = Σ k^k,
   then multiplying the prime powers together. */

#include <stdint.h>
#include "../utils/factorize.h"    /* factorize */
#include "../utils/mod_pow.h"      /* mod_pow */
#include "../utils/pow_uint.h"     /* pow_uint */
#include "../utils/seq_mul.h"      /* seq_mul */

/* Compute a(n) as defined.  */
/* External linkage as requested. */
seq_t a(idx_t n) {
    /* Handle n == 0: by convention a(0) = 0 (unused). */
    if (n == 0) {
        return 0;
    }

    /* Prime factorization of n.  */
    uint64_t primes[32];
    int exponents[32];
    size_t nf = factorize(n, primes, exponents); /* nf: number of distinct primes */

    seq_t result = 1;   /* accumulator for the gcd */

    /* For each distinct prime factor of n: compute its exponent in S(n). */
    for (size_t i = 0; i < nf; ++i) {
        uint64_t p = primes[i];
        int exp = 0;    /* exponent of p in S(n) */

        /* Increase exponent while p^(exp+1) divides S(n). */
        while (true) {
            /* Compute modulus m = p^(exp+1). */
            seq_t m = pow_uint(p, (idx_t)(exp + 1));

            /* Compute S(n) modulo m. */
            seq_t sum_mod = 0;
            for (idx_t k = 1; k <= n; ++k) {
                seq_t term = mod_pow((seq_t)k, k, m);
                sum_mod += term;
                if (sum_mod >= m) {
                    sum_mod -= m;
                }
            }

            /* If the sum is divisible by m, increase exp. */
            if (sum_mod == 0) {
                ++exp;
                /* Protect against runaway loops (unlikely for n <= 10000). */
                if (exp > 200) {   /* safety guard; should not happen */
                    break;
                }
            } else {
                break;    /* exponent found */
            }
        }

        if (exp > 0) {
            seq_t pow_val = pow_uint(p, (idx_t)exp);
            result = seq_mul(result, pow_val);
        }
    }

    return result;
}
