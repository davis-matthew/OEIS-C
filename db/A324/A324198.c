/*
 * A324198  a(n) = gcd(n, A276086(n)), where A276086 is the primorial base
 *          exp-function.
 * https://oeis.org/A324198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A276086(n))
 * Range:     u8 n <= 342, u16 n <= 9463, u32 n <= 9463, u64 n <= 9463,
 *            u128 n <= 9463, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A276086(n) is the primorial base exp‑function. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition. */
    if (n == (idx_t)0) return (seq_t)1;

    /* build list of primes up to n */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    /* compute primorial weights up to n */
    uint64_t weight[64];
    weight[0] = 1;
    size_t k = 0;
    while (k < prime_cnt && weight[k] <= n / primes[k]) {
        weight[k + 1] = weight[k] * primes[k];
        k++;
    }
    size_t max_k = k;                /* last valid weight index */

    /* primorial base digits of n */
    uint64_t digits[64] = {0};
    uint64_t rem = n;
    for (int i = (int)max_k; i >= 0; --i) {
        digits[i] = rem / weight[i];
        rem     %= weight[i];
    }

    /* factorize n */
    uint64_t fp[20], fe[20];
    size_t fc = 0;
    factor_u64(n, fp, fe, &fc);

    /* compute gcd via exponents */
    seq_t result = (seq_t)1;
    for (size_t idx = 0; idx < fc; ++idx) {
        uint64_t p   = fp[idx];
        uint64_t eN  = fe[idx];

        /* locate prime p in primes array */
        size_t j = 0;
        for (; j < prime_cnt; ++j) {
            if (primes[j] == p) break;
        }
        if (j >= prime_cnt) continue;   /* safety: should not happen */

        uint64_t eA = (j <= max_k) ? digits[j] : (uint64_t)0;
        uint64_t e  = (eN < eA) ? eN : eA;

        for (uint64_t t = 0; t < e; ++t) {
            result *= (seq_t)p;
        }
    }

    free(primes);
    return result;
}
