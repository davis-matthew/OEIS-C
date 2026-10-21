/*
 * A275534  Number of primes of the form x^2 + y^2 less than or equal to
 *          2*n^2.
 * https://oeis.org/A275534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p ≤ 2n² that are 2 or ≡ 1 (mod 4)
 * Range:     u8 n <= 43, u16 n <= 933, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form x² + y² are exactly 2 and primes ≡ 1 (mod 4). */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* out of range – undefined in OEIS */

    /* 2·n² fits in idx_t for n up to at least 10 000 */
    idx_t limit = (idx_t)2 * n * n;

    /* Cache the prime table up to the largest limit requested so far. */
    static idx_t max_limit = 0;
    static uint64_t *prime_list = NULL;
    static size_t prime_count = 0;

    if (limit > max_limit || prime_list == NULL) {
        if (prime_list) free(prime_list);
        build_primes_upto(limit, &prime_list, &prime_count);
        max_limit = limit;
    }

    seq_t count = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > limit) break;
        if (p == 2 || (p & 3) == 1) ++count;
    }
    return count;
}
