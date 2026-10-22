/*
 * A373383  a(n) = 1 if n and A059975(n) are both multiples of 3, otherwise 0,
 *          where A059975 is fully additive with a(p) = p-1.
 * https://oeis.org/A373383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n ≡ 0 (mod 3) and A059975(n) ≡ 0 (mod 3), else 0
 * Range:     u8 n <= 65, u16 n <= 9155, u32 n <= 9155, u64 n <= 9155,
 *            u128 n <= 9155, big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

seq_t a(idx_t n) {
    /* Quick reject if n is not divisible by 3 */
    if (n % 3u != 0u) return 0;

    /* Ensure we have primes up to sqrt(n) */
    seq_t nx = (seq_t)n;
    seq_t sqrt_n = floor_sqrt(nx);
    idx_t needed = (idx_t)(sqrt_n + 1);   /* +1 for safety */
    if (primes_limit < needed) {
        if (primes != NULL) free(primes);
        build_primes_upto(needed, &primes, &primes_cnt);
        primes_limit = needed;
    }

    /* Factor n, keeping track of exponent of 3 and exponents of primes ≡ 2 mod 3 */
    idx_t temp = n;
    unsigned e3 = 0;
    unsigned sum2 = 0;   /* count of exponents for primes ≡ 2 (mod 3) */

    while ((temp % 3u) == 0u) {
        temp /= 3u;
        e3++;
    }

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= temp; ++i) {
        idx_t p = primes[i];
        if (p == 3u) continue;          /* already handled */
        unsigned e = 0;
        while (temp % p == 0u) {
            temp /= p;
            e++;
        }
        if (e > 0 && (p % 3u) == 2u) sum2 += e;
    }

    if (temp > 1u) {
        /* Remaining prime factor */
        idx_t p = temp;
        if ((p % 3u) == 2u) sum2 += 1u;
    }

    /* Compute A059975(n) mod 3: 2*e3 + sum2 (mod 3) */
    unsigned mod3 = (2u * e3 + sum2) % 3u;
    return (seq_t)(mod3 == 0u);
}
