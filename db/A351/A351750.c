/*
 * A351750  a(n) = Sum_{p|n, p prime} p * sigma_p(p).
 * https://oeis.org/A351750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p * sigma_p(p) * = Sum_{p|n} (p +
 *            p^(p+1)) (sigma_p(p)=1^p + p^p) * * Implementation uses the
 *            supplied helper factor_u64 to obtain the * distinct prime
 *            factors of n and big_pow to compute p^(p+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;          /* 1 has no prime divisors, a(1)=0 */

    /* Factor n into distinct primes. We know there are far fewer than 64 primes
     * for n up to 10‑000, so a fixed recursion depth suffices.
     */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exponents, &cnt);

    seq_t result = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];          /* prime factor */
        seq_t p_seq = (seq_t)p;
        seq_t exp = (seq_t)(p + 1);      /* exponent p+1 */
        seq_t pow_p = big_pow(p_seq, exp);   /* p^(p+1) */
        result += pow_p + p_seq;          /* p + p^(p+1) */
    }
    return result;
}
