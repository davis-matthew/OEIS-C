/*
 * A349000  a(n) = A323166(A276086(n)), where A323166(n) = gcd(n, usigma(n)),
 *          usigma (A034448) is multiplicative with a(p^e) = (p^e)+1, and
 *          A276086 gives the prime product form of primorial base expansion
 *          of n.
 * https://oeis.org/A349000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A323166(A276086(n))
 * Range:     u8 n <= 14, u16 n <= 58, u32 n <= 826, u64 n <= 9862,
 *            u128 n <= 9862, big any n
 * Helpers:   factor_u64, gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/seq_pow.h"
#include "../utils/factor_u64.h"

static const uint64_t PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97
};
static const size_t NUM_PRIMES = sizeof(PRIMES) / sizeof(PRIMES[0]);

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Build primorial weights less than or equal to n */
    seq_t weights[NUM_PRIMES + 1];
    size_t weight_cnt = 1;          /* weights[0] = 1 */
    weights[0] = (seq_t)1;
    seq_t w = (seq_t)1;

    for (size_t i = 0; i < NUM_PRIMES; ++i) {
        seq_t next = w * (seq_t)PRIMES[i];
        if (next > (seq_t)n) {
            break;
        }
        w = next;
        weights[weight_cnt++] = w;
    }

    /* Compute A276086(n) = product of primes^digits */
    seq_t m = (seq_t)1;
    idx_t remaining = n;

    for (size_t idx = weight_cnt; idx > 0; --idx) {
        size_t i = idx - 1;            /* prime index */
        seq_t weight = weights[i];
        idx_t digit = remaining / (idx_t)weight;
        remaining %= (idx_t)weight;
        if (digit > 0) {
            seq_t p_pow = seq_pow((seq_t)PRIMES[i], (idx_t)digit);
            m *= p_pow;
        }
    }

    /* Compute u‑sigma(m) = ∏ (1 + p^e) over prime factorisation of m */
    seq_t usigma = (seq_t)1;

    /* Factorise m (fits in 64 bits for the intended range of n) */
    uint64_t m64 = (uint64_t)m;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(m64, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p_pow = seq_pow((seq_t)pr[i], (idx_t)ex[i]);
        usigma *= (p_pow + (seq_t)1);
    }

    /* Final result: gcd(m, usigma) */
    return gcd(m, usigma);
}
