/*
 * A287592  Number of connected induced subgraphs on the n-crown graph.
 * https://oeis.org/A287592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1 + 2^n)^2 - ( n * ( -7 + 2^(1+n) + n )) / 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the closed‑form expression, with a small special‑case for n = 0,1
   to avoid signed intermediate problems. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 2;

    /* compute 2^n and 2^(n+1) */
    seq_t pow2n  = pow2(n);
    seq_t pow2n1 = pow2(n + 1);

    /* first term: (-1 + 2^n)^2 */
    seq_t t1 = pow2n - (seq_t)1;
    seq_t term1 = t1 * t1;

    /* second term: n * (2^(n+1) + n - 7) / 2 */
    seq_t inner = pow2n1 + (seq_t)n;      /* 2^(n+1) + n */
    inner = inner - (seq_t)7;             /* 2^(n+1) + n - 7 */
    seq_t term2 = (inner * (seq_t)n) / (seq_t)2;

    return term1 - term2;
}
