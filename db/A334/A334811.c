/*
 * A334811  Length of shortest overlap-free binary word having n periods.
 * https://oeis.org/A334811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   * a(1) = 1 * a(2) = 2 * a(3) = 5 * a(n) = (17 * 2^(2n-7) + 2) /
 *            3 for n >= 4
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-01)
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
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;

    /* Exponent for the power of two: 2n - 7 */
    idx_t exp = 2 * n - 7;

    /* Compute 2^(2n-7) */
    seq_t pw = ipow_idx((seq_t)2, exp);

    /* Compute 17 * 2^(2n-7) + 2 */
    seq_t numer = mul_small(pw, 17);
    numer = numer + (seq_t)2;

    /* Final result: divide by 3 */
    return numer / (seq_t)3;
}
