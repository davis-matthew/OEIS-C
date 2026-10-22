/*
 * A375931  The product of the prime powers in the prime factorization of n
 *          that have an exponent that is equal to the maximum exponent in
 *          this factorization.
 * https://oeis.org/A375931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{p^e||n, e = max_i e_i} p^e
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* a(0) is undefined, return 0 for safety */
    if (n == 0) return 0;

    /* 64‑bit containers for the factorisation; 64 primes is plenty for uint64_t */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factorise n (primes[i]^exps[i] are the prime powers in n) */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Find the maximum exponent in the factorisation */
    uint64_t maxe = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] > maxe) maxe = exps[i];
    }

    /* Multiply the prime powers whose exponent equals the maximum */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] == maxe) {
            /* seq_pow returns the power in the seq_t type */
            seq_t term = seq_pow((seq_t)primes[i], exps[i]);
            result = result * term;
        }
    }

    return result;
}
