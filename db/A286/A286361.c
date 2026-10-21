/*
 * A286361  Least number with the same prime signature as {the largest divisor
 *          of n with only prime factors of the form 4k+1} has: a(n) =
 *          A046523(A170818(n)).
 * https://oeis.org/A286361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046523(A170818(n)) Computes the least integer having the
 *            same prime signature as the largest divisor of n composed only
 *            of primes ≡ 1 mod 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* External linkage as requested. */
seq_t a(idx_t n)
{
    /* Factor n and collect exponents of primes ≡ 1 (mod 4). */
    uint64_t m = (uint64_t)n;
    size_t exps_cap = 8;
    size_t exps_len = 0;
    uint32_t *exps = (uint32_t *)malloc(exps_cap * sizeof(uint32_t));
    if (!exps) return (seq_t)0; /* allocation failure */

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint32_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (p % 4 == 1) {
                if (exps_len == exps_cap) {
                    exps_cap *= 2;
                    exps = (uint32_t *)realloc(exps, exps_cap * sizeof(uint32_t));
                    if (!exps) return (seq_t)0;
                }
                exps[exps_len++] = e;
            }
        }
    }
    if (m > 1) {               /* remaining prime factor */
        if (m % 4 == 1) {
            if (exps_len == exps_cap) {
                exps_cap *= 2;
                exps = (uint32_t *)realloc(exps, exps_cap * sizeof(uint32_t));
                if (!exps) return (seq_t)0;
            }
            exps[exps_len++] = 1;
        }
    }

    if (exps_len == 0) {       /* no 1 mod 4 primes in n */
        free(exps);
        return (seq_t)1;
    }

    /* Sort exponents descending (largest first). */
    for (size_t i = 1; i < exps_len; ++i) {
        uint32_t key = exps[i];
        size_t j = i;
        while (j > 0 && exps[j - 1] < key) {
            exps[j] = exps[j - 1];
            --j;
        }
        exps[j] = key;
    }

    /* Small list of the first few primes, enough for the required size. */
    static const uint32_t small_primes[20] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71
    };

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < exps_len; ++i) {
        uint32_t e = exps[i];
        seq_t p = (seq_t)small_primes[i];
        for (uint32_t k = 0; k < e; ++k) {
            result *= p;
        }
    }

    free(exps);
    return result;
}
