/*
 * A278253  Least number with the prime signature of the n-th triangular
 *          number.
 * https://oeis.org/A278253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046523(A000217(n)) A000217(n) is the n-th triangular
 *            number n*(n+1)/2. A046523(m) returns the least number with the
 *            same prime signature as m (exponents of primes sorted
 *            decreasingly, assigned to the smallest primes)
 * Range:     u8 n <= 22, u16 n <= 382, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute the n-th triangular number */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* factorize t as a 64‑bit number (sufficient for n up to ~10 000) */
    uint64_t m = (uint64_t)t;          /* tmp work variable */

    /* array of exponents of prime factors */
    int exponents[64];
    int ex_count = 0;

    uint64_t temp = m;
    int exp = 0;
    while ((temp & 1ULL) == 0) {       /* factor 2 */
        exp++;
        temp >>= 1;
    }
    if (exp > 0) exponents[ex_count++] = exp;

    for (uint64_t p = 3; p * p <= temp; p += 2) {  /* odd primes */
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) {
                exp++;
                temp /= p;
            }
            exponents[ex_count++] = exp;
        }
    }
    if (temp > 1)                       /* remaining prime > sqrt */
        exponents[ex_count++] = 1;

    /* sort exponents in decreasing order */
    for (int i = 0; i < ex_count - 1; ++i)
        for (int j = i + 1; j < ex_count; ++j)
            if (exponents[i] < exponents[j]) {
                int tmp = exponents[i];
                exponents[i] = exponents[j];
                exponents[j] = tmp;
            }

    /* first few primes: enough for the expected exponents */
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67,
        71, 73, 79, 83, 89, 97, 101, 103,
        107, 109, 113
    };

    seq_t result = 1;
    for (int i = 0; i < ex_count; ++i) {
        uint64_t prime = small_primes[i];
        for (int k = 0; k < exponents[i]; ++k)
            result *= (seq_t)prime;
    }
    return result;
}
