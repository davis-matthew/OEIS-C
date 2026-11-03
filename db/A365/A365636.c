/*
 * A365636  a(n) is the smallest multiple of n that is a term of A072873.
 * https://oeis.org/A365636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative. For a prime power p^e, * a(p^e) =
 *            p^(p*ceil(e/p)). This is the smallest multiple of * n that
 *            belongs to A072873
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 381 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, but safety */
    if (n == 1) return 1;

    /* Factorization: primes and exponents */
    size_t cnt;
    uint64_t pr[64], ex[64];
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        /* exponent for the A072873 term */
        uint64_t ep = p * ((e + p - 1) / p);  /* p * ceil(e/p) */
        result = result * ipow((seq_t)p, (seq_t)ep);
    }
    return result;
}
