/*
 * A295921  Number of (not necessarily maximal) cliques in the n-folded cube
 *          graph.
 * https://oeis.org/A295921
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2)*(n+2) + 1 for n > 3, with a(2) = 4, a(3) = 16.
 *            The offset is 2
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 32 (2026-11-01)
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
    /* Undefined for n < 2; return 0 for safety. */
    if (n < 2) return (seq_t)0;

    /* Handle the small fixed terms. */
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)16;

    /* For n >= 4 use the closed‑form formula. */
    seq_t power = pow2(n - 2);                     /* 2^(n-2) */
    seq_t mult  = mul_small(power, (unsigned)(n + 2));
    return mult + (seq_t)1;
}
