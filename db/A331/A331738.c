/*
 * A331738  Multiplicative with a(p^e) = p^(e-A000265(e)), where A000265(x)
 *          gives the odd part of x.
 * https://oeis.org/A331738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} p^(e - A000265(e)) where A000265(e) =
 *            odd_part(e)
 * Range:     u8 n <= 2303, u16 n <= 9939, u32 n <= 9939, u64 n <= 9939,
 *            u128 n <= 9939, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* arrays large enough for factorisation of 64‑bit numbers */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;

    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];

        /* odd part of e */
        uint64_t m = e;
        while ((m & 1ULL) == 0)
            m >>= 1;

        uint64_t exp = e - m; /* exponent to contribute to the product */

        seq_t base = (seq_t)p;
        for (uint64_t k = 0; k < exp; ++k)
            res *= base;
    }
    return res;
}
