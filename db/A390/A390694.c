/*
 * A390694  The smallest multiple of n that is a Loeschian number (A003136).
 * https://oeis.org/A390694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n * ∏_{p≡2 (mod 3), e odd} p, where n = ∏ p^e Equivalent
 *            multiplicative definition: a(p^e) = p^(e+(e mod 2)) if p≡2 (mod
 *            3), otherwise a(p^e) = p^e
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)n;          /* a(1) = 1 */

    /* Sufficiently large workspace for uint64 factorization */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* decompose n into prime powers */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = (seq_t)n;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if ((p % 3ULL) == 2ULL && (e & 1ULL)) {
            /* p ≡ 2 (mod 3) and exponent e is odd → multiply by one more p */
            res = res * (seq_t)p;
        }
    }
    return res;
}
