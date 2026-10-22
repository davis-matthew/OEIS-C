/*
 * A384796  Combined output of the Wichmann-Hill pseudo-random number
 *          generator multiplied by the product of its 3 internal moduli,
 *          27817185604309.
 * https://oeis.org/A384796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27817185604309 * frac(A385031(n)/30269 + A385032(n)/30307
 *            + A385033(n)/30323)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      where frac(x) = x - floor(x)  and A385031, A385032, A385033 are the three
 *      streams of the Wichmann–Hill pseudo‑random number generator with multipliers
 *      171, 172 and 170 respectively, started at 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Moduli of the three component generators */
    const uint64_t MOD1 = 30269ULL;
    const uint64_t MOD2 = 30307ULL;
    const uint64_t MOD3 = 30323ULL;

    /* Pre‑computed constants for the numerator:
     *      result = (u1*MOD2*MOD3 + u2*MOD1*MOD3 + u3*MOD1*MOD2) mod PROD
     */
    const uint64_t COEF1 = MOD2 * MOD3;   /* 918999161 */
    const uint64_t COEF2 = MOD1 * MOD3;   /* 917846887 */
    const uint64_t COEF3 = MOD1 * MOD2;   /* 917362583 */

    /* Product of the three moduli */
    const uint64_t PROD = 27817185604309ULL;   /* 30269 * 30307 * 30323 */

    uint64_t u1 = 1, u2 = 1, u3 = 1;   /* initial seeds */
    seq_t   result = 0;

    for (idx_t i = 1; i <= n; ++i) {
        /* Numerator of the fractional sum, modulo PROD */
        uint64_t num = u1 * COEF1 + u2 * COEF2 + u3 * COEF3;
        result = (seq_t)(num % PROD);

        /* Advance the three component generators */
        u1 = (171ULL * u1) % MOD1;
        u2 = (172ULL * u2) % MOD2;
        u3 = (170ULL * u3) % MOD3;
    }
    return result;
}
