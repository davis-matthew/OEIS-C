/*
 * A258073  a(n) = 1 + 78557*2^n.
 * https://oeis.org/A258073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 78557*2^n (offset 1)
 * Range:     u8 none, u16 none, u32 n <= 15, u64 n <= 47, u128 n <= 111,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 992 (2026-11-01)
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
    /* n >= 1 by definition of the sequence */
    seq_t power = pow2(n);           /* 2^n  */
    seq_t prod  = mul_small(power, 78557); /* 78557 * 2^n */
    return prod + 1;                /* a(n) = 1 + 78557*2^n */
}
