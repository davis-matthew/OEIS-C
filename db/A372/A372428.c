/*
 * A372428  Sum of binary indices of n minus sum of prime indices of n.
 * https://oeis.org/A372428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = (sum of binary indices of n) - (sum of prime indices of
 *            n)
 * Range:     i8 n <= 1030, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For completeness we simply
       return 0 for the undefined case n == 0. */
    if (n == 0) return (seq_t)0;

    /* ---------- 1. sum of binary indices ---------- */
    seq_t sumBits = 0;
    idx_t tmp = n;
    idx_t pos = 1;
    while (tmp != 0) {
        if (tmp & 1ULL) sumBits += (seq_t)pos;
        tmp >>= 1;
        pos++;
    }

    /* ---------- 2. generate primes up to n ---------- */
    size_t limit = (size_t)n;
    if (limit < 2) limit = 2;              /* at least 2 to have a sieve */
    unsigned char *is_prime = (unsigned char *)calloc(limit + 1, sizeof(unsigned char));
    int *prime_idx       = (int *)calloc(limit + 1, sizeof(int));
    if (!is_prime || !prime_idx) {        /* memory allocation failure */
        free(is_prime);
        free(prime_idx);
        return sumBits;                   /* fall back to sumBits only */
    }

    /* initialise all entries as prime */
    for (size_t i = 2; i <= limit; ++i) is_prime[i] = 1;

    /* classical sieve */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* record the index of each prime (prime(1)=2, prime(2)=3, …) */
    int idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            idx++;
            prime_idx[i] = idx;
        }
    }

    /* ---------- 3. sum of prime indices of n (with multiplicity) ---------- */
    seq_t sumPrimeIdx = 0;
    tmp = n;
    /* factorisation: try all primes up to sqrt(tmp) */
    for (size_t p = 2; (size_t)p * p <= tmp; ++p) {
        if (is_prime[p]) {
            while (tmp % p == 0ULL) {
                sumPrimeIdx += (seq_t)prime_idx[p];
                tmp /= p;
            }
        }
    }
    /* if a prime factor larger than sqrt(n) remains */
    if (tmp > 1ULL) {
        /* tmp is a prime itself and, because tmp ≤ n, it is indexed */
        sumPrimeIdx += (seq_t)prime_idx[(size_t)tmp];
    }

    /* free temporary tables */
    free(is_prime);
    free(prime_idx);

    return sumBits - sumPrimeIdx;
}
