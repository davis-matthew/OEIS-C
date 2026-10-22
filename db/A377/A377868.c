/*
 * A377868  a(n) = A359550(A276085(n)), where A359550 is multiplicative with
 *          a(p^e) = 1 if p > e, otherwise 0, and A276085 is fully additive
 *          with a(p) = p#/p.
 * https://oeis.org/A377868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9805, u16 n <= 9805, u32 n <= 9805, u64 n <= 9805,
 *            u128 n <= 9805, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 0  if  (e2 + 2*odd_count) ≡ 0 (mod 4)
 *           a(n) = 1  otherwise
 *
 * where e2        = exponent of 2 in the prime factorisation of n,
 *       odd_count = number of primes p≥3 with odd exponent in n.
 *
 * This implements a(n) = A359550(A276085(n)) without computing
 * the large intermediate values, using only prime factorisation
 * of n.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;                 /* a(1) = 0 by definition */

    /* Allocate arrays large enough for all prime factors of a 64‑bit number */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    /* Factorise n (valid for idx_t == uint64_t) */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t e2        = 0;   /* exponent of prime 2 */
    uint64_t odd_count = 0;   /* number of primes ≥3 with odd exponent */

    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 2)
            e2 = exps[i];
        else
            odd_count += (exps[i] & 1ULL);   /* add 1 if exponent is odd */
    }

    uint64_t value = e2 + 2ULL * odd_count;
    return (value & 3ULL) == 0 ? (seq_t)0 : (seq_t)1;
}
