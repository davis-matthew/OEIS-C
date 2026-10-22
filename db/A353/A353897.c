/*
 * A353897  a(n) is the largest divisor of n whose exponents in its prime
 *          factorization are all powers of 2 (A138302).
 * https://oeis.org/A353897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e || n of
 *            p^(2^floor(log2(e)))
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* return the largest power of two that does not exceed e */
static uint64_t highest_power_of_two_le(uint64_t e) {
    uint64_t v = 1;
    while ((v << 1) <= e) v <<= 1;
    return v;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)n;                /* a(1) = 1 */

    uint64_t N = (uint64_t)n;
    /* a 64‑bit integer has at most 16 distinct prime factors, 64 is safe */
    uint64_t primes_buf[64];
    uint64_t exps_buf[64];
    size_t cnt;

    factor_u64(N, primes_buf, exps_buf, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes_buf[i];
        uint64_t e  = exps_buf[i];
        uint64_t exp2 = highest_power_of_two_le(e);
        result *= big_pow((seq_t)p, (seq_t)exp2);
    }
    return result;
}
