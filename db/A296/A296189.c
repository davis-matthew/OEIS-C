/*
 * A296189  Harary index for the n-Keller graph.
 * https://oeis.org/A296189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n*(4^n - (3^n + n + 1)/2)/2 (n>1), a(1)=0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* 4^n and 3^n */
    seq_t A = pow4(n);      /* 4^n */
    seq_t B = pow3(n);      /* 3^n */

    /* C = 3^n + n + 1 */
    seq_t C = B + (seq_t)n + (seq_t)1;

    /* 2*A - C  (all 64‑bit safe because 2*A fits in 64 bits for n≤10000) */
    seq_t twoA = A << 1;
    seq_t diff = twoA - C;

    /* A/4 = 4^(n-1)  (4^n is divisible by 4 for n>=1) */
    seq_t A_div4 = A >> 2;

    /* Final value: (A/4) * (2*A - C)  (fits in seq_t) */
    return A_div4 * diff;
}
