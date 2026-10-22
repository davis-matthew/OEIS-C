/*
 * A338238  Minimum number of rotations for a second maximum cyclic
 *          autocorrelation of the first n terms of the characteristic
 *          function of primes.
 * https://oeis.org/A338238
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k (0 ≤ k < n) for which the cyclic *
 *            autocorrelation C[k] of the characteristic function of primes *
 *            (up to n) equals the second highest value among all C[0…n‑1]. *
 *            * C[k] is defined by * * C[k] = Σ_{i=1}^{n} f[i] * f[(i+k-1) mod
 *            n + 1] * * where f[i] = 1 if i is prime, otherw...
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* We only care about n ≥ 2 */
    if (n < 2)
        return (seq_t)0;

    /* ---------- Prime sieve up to n ---------- */
    size_t sz = (size_t)n + 1;
    uint8_t *is_prime = (uint8_t*)calloc(sz, sizeof(uint8_t));
    if (!is_prime)
        return (seq_t)0;

    /* mark odds as prime, 1 is not prime */
    if (n >= 2)
        is_prime[2] = 1;
    for (idx_t i = 3; i <= n; i += 2)
        is_prime[i] = 1;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t k = p * p; k <= n; k += p)
                is_prime[k] = 0;
        }
    }

    /* ---------- Collect indices of primes ---------- */
    size_t primes_cnt = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (is_prime[i])
            ++primes_cnt;

    idx_t *primes = (idx_t*)malloc(primes_cnt * sizeof(idx_t));
    if (!primes) {
        free(is_prime);
        return (seq_t)0;
    }

    size_t pos = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (is_prime[i])
            primes[pos++] = i;

    free(is_prime);

    /* ---------- Compute cyclic autocorrelations ---------- */
    seq_t *C = (seq_t*)calloc((size_t)n, sizeof(seq_t));
    if (!C) {
        free(primes);
        return (seq_t)0;
    }

    for (size_t ii = 0; ii < primes_cnt; ++ii) {
        idx_t pi = primes[ii];
        for (size_t jj = 0; jj < primes_cnt; ++jj) {
            idx_t q = primes[jj];
            size_t d = (q >= pi) ? (q - pi) : (q + n - pi);
            C[d] += (seq_t)1;
        }
    }

    /* ---------- Find second‑largest value among all C ---------- */
    seq_t M1 = C[0];
    size_t cntM1 = 1;          /* number of k with value == M1 */
    for (idx_t k = 1; k < n; ++k) {
        if (C[k] > M1) {
            M1 = C[k];
            cntM1 = 1;
        } else if (C[k] == M1) {
            ++cntM1;
        }
    }

    seq_t M2;
    if (cntM1 >= 2) {
        /* There is more than one rotation with the maximal value. */
        M2 = M1;
    } else {
        /* Second maximum is the largest value smaller than M1. */
        M2 = (seq_t)0;
        for (idx_t k = 0; k < n; ++k) {
            if (C[k] < M1 && C[k] > M2)
                M2 = C[k];
        }
    }

    /* ---------- Find smallest k that achieves M2 ---------- */
    idx_t result = 0;
    for (idx_t k = 0; k < n; ++k) {
        if (C[k] == M2) {
            result = k;
            break;
        }
    }

    free(primes);
    free(C);
    return (seq_t)result;
}
