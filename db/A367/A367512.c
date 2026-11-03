/*
 * A367512  Number of evil exponents in the prime factorization of n.
 * https://oeis.org/A367512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of prime exponents e of n that are evil (i.e., e
 *            has an even number of 1‑bits in binary)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9005 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                 /* for size_t */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static inline seq_t _is_evil_exponent(uint64_t e)
{
    /* An exponent is evil if its binary representation has an even
       number of 1‑bits.  __builtin_popcountll is available in GCC/Clang
       and is evaluated at compile time if e is a constant. */
    return ((__builtin_popcountll(e) & 1ULL) ? (seq_t)0 : (seq_t)1);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety: we care about n ≥ 1 only */

    /* Factorization of n; we only need the exponents. */
    uint64_t primes[64];            /* enough for any 64‑bit n */
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum += _is_evil_exponent(exps[i]);
    }
    return sum;
}
