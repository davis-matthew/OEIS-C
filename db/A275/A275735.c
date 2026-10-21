/*
 * A275735  Prime-factorization representations of "factorial base level
 *          polynomials": a(0) = 1; for n >= 1, a(n) = 2^A257511(n) *
 *          A003961(a(A257684(n))).
 * https://oeis.org/A275735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = ∏_{k ≥ 1} prime(k)^{count of digit k in factorial base
 *            representation of n}
 * Range:     u8 n <= 214, u16 n <= 9812, u32 n <= 9812, u64 n <= 9812,
 *            u128 n <= 9812, big any n
 * Helpers:   get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Precomputed factorials up to 20! (fits in 64‑bit) */
    static uint64_t fact[21];
    static bool init = false;
    if (!init) {
        fact[0] = 1;
        for (int i = 1; i <= 20; ++i) fact[i] = fact[i-1] * (uint64_t)i;
        init = true;
    }

    /* Find the largest j with j! <= n */
    int maxj = 1;
    while (maxj < 20 && fact[maxj + 1] <= (uint64_t)n) ++maxj;

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    for (int j = maxj; j >= 1; --j) {
        uint64_t digit = m / fact[j];
        m %= fact[j];
        if (digit != 0) {
            seq_t prime = (seq_t)get_prime((idx_t)digit);
            result = seq_mul(result, prime);  /* multiply once per occurrence */
        }
    }
    return result;
}
