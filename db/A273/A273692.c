/*
 * A273692  a(n) is the denominator of 2*O(n+1) - O(n+2) where O(n) = n/2^n,
 *          the n-th Oresme number.
 * https://oeis.org/A273692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (2+3*n)/2^(n+2) = 2^(n+2 - v2(3n+2))
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 2757 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are defined elsewhere by the build system. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Count the exponent of 2 dividing 3*n+2, using 128-bit arithmetic to avoid overflow. */
static inline unsigned int exp2_3n2(idx_t n)
{
    unsigned __int128 val = (unsigned __int128)3 * n + 2; /* compute 3n+2 safely */
    unsigned int v = 0;
    while ((val & 1) == 0 && v < 128) { /* enough for idx_t up to 64 bits */
        v++;
        val >>= 1;
    }
    return v;
}

seq_t a(idx_t n)
{
    /* Compute v = v2(3n+2) and the exponent for the denominator. */
    unsigned int v = exp2_3n2(n);
    /* The denominator is 2^(n+2 - v), but not larger than 2^(n+2). */
    unsigned int exp = (unsigned int)(n + 2) - v;
    return pow2(exp);
}
