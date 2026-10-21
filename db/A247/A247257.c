/*
 * A247257  The number of octic characters modulo n.
 * https://oeis.org/A247257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = ∏_p a(p^e) with a(2^e) = 2^min(e−1,4) and a(p^e) =
 *            gcd(8,p−1) for odd p
 * Range:     u8 n <= 1359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, xor_gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/xor_gcd64.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n==1 */
    if (n == 1)
        return (seq_t)1;

    /* Prime factorisation buffer – sufficient for n up to very large values */
    uint64_t pr[64], ex[64];
    size_t   cnt = 0;

    factor_u64(n, pr, ex, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = pr[i];
        uint64_t e   = ex[i];
        if (p == 2) {
            /* 2^min(e−1,4) */
            uint64_t m = (e > 1 ? e - 1 : 0);
            if (m > 4) m = 4;
            /* 2^m fits into seq_t for m≤4 (max 16) */
            result *= (seq_t)(1ULL << m);
        } else {
            /* gcd(8,p−1), always ≤8 */
            uint64_t g = xor_gcd64(8ULL, p - 1ULL);
            result *= (seq_t)g;
        }
    }
    return result;
}
