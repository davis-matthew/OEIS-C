/*
 * A250608  Number of (n+1) X (5+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 144*2^(n-1) + 42*n^2 + 69*n - 8
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 192 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

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
    /* Ensure the input is at least 1 (the sequence offset) */
    if (n < 1)
        return 0;

    seq_t nseq = (seq_t)n;

    /* 144 * 2^(n-1) */
    seq_t power = ipow_i32((seq_t)2, (int)(n - 1));
    power = mul_small(power, 144);

    /* 42*n^2 + 69*n */
    seq_t poly = (seq_t)42 * (nseq * nseq) + (seq_t)69 * nseq;

    /* Combine and subtract the constant offset 8 */
    return power + poly - (seq_t)8;
}
