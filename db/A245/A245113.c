/*
 * A245113  G.f. A(x) satisfies A(x)^2 = 1 + 4*x*A(x)^6.
 * https://oeis.org/A245113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * binomial(3*n+1/2, n) / (6*n+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Update exponent array: add sign * exponent of val's prime factors */
static void factor_and_update(uint64_t val, int sign,
                              const uint64_t *primes, size_t prime_cnt,
                              int *exp)
{
    uint64_t x = val;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= x; ++i) {
        if (x % primes[i] == 0) {
            int c = 0;
            while (x % primes[i] == 0) {
                x /= primes[i];
                ++c;
            }
            exp[i] += sign * c;
        }
    }
    if (x > 1) {
        /* x is a prime > sqrt(val) */
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (primes[mid] < x) lo = mid + 1;
            else hi = mid;
        }
        if (lo < prime_cnt && primes[lo] == x)
            exp[lo] += sign;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    uint64_t N = (uint64_t)n;
    uint64_t maxV = 6 * N + 1;

    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(maxV, &primes, &prime_cnt);
    if (!primes || prime_cnt == 0) return (seq_t)0;

    int *exp = (int *)calloc(prime_cnt, sizeof(int));
    if (!exp) {
        free(primes);
        return (seq_t)0;
    }

    /* denominator: (i+1) for i = 1..N and the extra factor (6N+1) */
    for (uint64_t i = 1; i <= N; ++i)
        factor_and_update(i, -1, primes, prime_cnt, exp);
    factor_and_update(6 * N + 1, -1, primes, prime_cnt, exp);

    /* numerator: product over i = 0..N-1 of (6N+1 - 2i) */
    for (uint64_t i = 0; i < N; ++i) {
        uint64_t v = 6 * N + 1 - 2 * i;
        factor_and_update(v, +1, primes, prime_cnt, exp);
    }

    /* multiply by 2^N (add N to exponent of prime 2) */
    size_t idx2 = 0;
    while (idx2 < prime_cnt && primes[idx2] != 2) ++idx2;
    if (idx2 < prime_cnt)
        exp[idx2] += (int)N;

    /* compute result from prime exponents */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        int e = exp[i];
        if (e > 0) {
            seq_t pw = ipow_idx((seq_t)primes[i], (idx_t)e);
            result = seq_mul(result, pw);
        }
    }

    free(exp);
    free(primes);

    return result;
}
