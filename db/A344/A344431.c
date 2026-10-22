/*
 * A344431  a(n) = Sum_{k=1..n} mu(k) * n^(k - 1).
 * https://oeis.org/A344431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} mu(k) * n^(k-1)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* --- Möbius sieve up to n --- */
    size_t N  = (size_t)n;
    size_t sz = N + 1;

    int8_t  *mu      = (int8_t *)calloc(sz, sizeof(int8_t));
    uint8_t *iscomp  = (uint8_t *)calloc(sz, sizeof(uint8_t));
    idx_t   *primes  = (idx_t   *)malloc(sz * sizeof(idx_t));

    if (!mu || !iscomp || !primes) {
        free(mu); free(iscomp); free(primes);
        return 0;
    }

    mu[1] = 1;
    size_t pcnt = 0;

    for (size_t i = 2; i <= N; ++i) {
        if (!iscomp[i]) {
            primes[pcnt++] = (idx_t)i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            idx_t p   = primes[j];
            idx_t t   = (idx_t)i * p;
            if (t > n) break;
            iscomp[t] = 1;
            if (i % p == 0) {
                mu[t] = 0;
                break;
            } else {
                mu[t] = -mu[i];
            }
        }
    }

    /* --- Compute the summed value --- */
    seq_t sum   = 0;
    seq_t power = (seq_t)1;
    seq_t nseq  = (seq_t)n;

    for (size_t k = 1; k <= N; ++k) {
        sum += (seq_t)mu[k] * power;
        power *= nseq;
    }

    /* --- Clean up --- */
    free(mu);
    free(iscomp);
    free(primes);

    return sum;
}
