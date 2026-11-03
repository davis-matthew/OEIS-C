/*
 * A385085  a(n) = 2 * (3*n+2)^(n-1).
 * https://oeis.org/A385085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (3*n+2)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;            /* a(0) = 1 */

    /* base = 3*n + 2 */
    seq_t base = (seq_t)3 * (seq_t)n + (seq_t)2;

    /* exponent = n-1 */
    seq_t exp = (seq_t)(n - 1);

    /* Compute  (3*n+2)^(n-1) and then multiply by 2 */
    seq_t result = ipow(base, exp);
    result = mul_small(result, 2U);

    return result;
}
