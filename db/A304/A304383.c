/*
 * A304383  a(n) = 36*2^n - 5 (n>=1).
 * https://oeis.org/A304383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*2^n - 5
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 1000 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"

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
    if (n == 0) /* undefined for OEIS offset 1, but return 0 for safety */
        return (seq_t)0;
    seq_t val = pow2(n);          /* compute 2^n */
    val = mul_small(val, 36);      /* multiply by 36 */
    val -= (seq_t)5;               /* subtract 5 */
    return val;
}
