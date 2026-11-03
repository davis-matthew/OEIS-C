/*
 * A266090  Decimal representation of the n-th iteration of the "Rule 17"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(n/2) for even n; a(n) = 2*4^n - 6*8^((n-1)/2) - 1 for
 *            odd n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 454 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

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
    /* Even indices */
    if ((n & 1) == 0) {
        idx_t k = n >> 1;               /* k = n/2 */
        return ipow_idx((seq_t)8, k);    /* 8^k */
    }

    /* Odd indices */
    idx_t half = (n - 1) >> 1;          /* (n-1)/2 */
    seq_t term1 = mul_small(ipow_idx((seq_t)4, n), 2);          /* 2*4^n */
    seq_t term2 = mul_small(ipow_idx((seq_t)8, half), 6);       /* 6*8^((n-1)/2) */
    seq_t res = term1 - term2;          /* term1 - term2 */
    res = res - (seq_t)1;               /* subtract 1 */
    return res;
}
