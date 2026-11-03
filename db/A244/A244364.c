/*
 * A244364  a(n) is the least integer m > 1 such that n is the largest number
 *          of identical digits that can end m^k for positive integer k.
 * https://oeis.org/A244364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, hard, more
 * Formula:   a(n) = 10*2^(n-1)-1 for n>5; a(1)=3, a(2)=4, a(3)=2, a(4)=33,
 *            a(5)=17
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-01)
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
    /* initial values for the first 5 indices */
    static const seq_t init[] = { 3, 4, 2, 33, 17 };
    if (n >= 1 && n <= 5)
        return init[n - 1];

    /* closed‑form for n > 5 */
    seq_t p = pow2(n - 1);          /* 2^(n‑1) */
    p = mul_small(p, 10);           /* *10 */
    return p - 1;                   /* -1 */
}
