/*
 * A360460  Two times the median of the unordered prime signature of n; a(1) =
 *          1.
 * https://oeis.org/A360460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * median of the multiset of prime exponents in n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Static cache of primes up to 10000 for factorisation */
static bool primes_built = false;
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;

seq_t a(idx_t n) {
    /* Special case for n=1 as defined in the OEIS entry */
    if (n == (idx_t)1) {
        return (seq_t)1;
    }

    /* Build a list of primes once, sufficient for n up to about 1e8 */
    if (!primes_built) {
        /* 10000 is > sqrt(1e8) and comfortably covers n up to 10000 */
        build_primes_upto(10000ULL, &primes, &prime_cnt);
        primes_built = true;
    }

    uint64_t temp = (uint64_t)n;
    int exponents[64];   /* store exponents of prime factors */
    int m = 0;           /* number of distinct prime factors */

    /* Trial division using the cached primes */
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0ULL) {
            int cnt = 0;
            while (temp % p == 0ULL) {
                temp /= p;
                ++cnt;
            }
            exponents[m++] = cnt;
        }
    }

    /* Remaining factor (if any) is a single large prime */
    if (temp > 1ULL) {
        exponents[m++] = 1;
    }

    /* Sort the exponents (they are already small, n < 10^6) */
    for (int i = 0; i < m - 1; ++i) {
        for (int j = i + 1; j < m; ++j) {
            if (exponents[i] > exponents[j]) {
                int t = exponents[i];
                exponents[i] = exponents[j];
                exponents[j] = t;
            }
        }
    }

    seq_t result;
    if (m % 2 == 1) { /* odd number of exponents */
        result = (seq_t)(2 * exponents[m / 2]);
    } else {          /* even number of exponents */
        result = (seq_t)(exponents[m / 2 - 1] + exponents[m / 2]);
    }

    return result;
}
