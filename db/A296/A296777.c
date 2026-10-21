/*
 * A296777  Detour index of the n-cube-connected cycle graph.
 * https://oeis.org/A296777
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^(n‑1)*n*((2^n*n – 1)^2) for n odd, a(n) =
 *            2^(n‑1)*n*((2^n*n – 1)^2 – (2^(n‑1)*n – 1)) for n even (Offset
 *            3: a(3)=6348, a(4)=126016, a(5)=2022480)
 * Range:     u8 none, u16 n <= 3, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;           /* values below offset are undefined */

    /* 2^(n-1) and 2^n */
    seq_t p2_nminus1 = pow2(n - 1);
    seq_t p2_n       = pow2(n);

    /* 2^n * n  */
    seq_t prod = p2_n * (seq_t)n;

    /* (2^n*n - 1) */
    seq_t val = prod - 1;

    /* (2^n*n - 1)^2 */
    seq_t sq = val * val;

    /* For even n subtract (2^(n-1)*n - 1) from the square term */
    if ((n & 1) == 0) {
        seq_t sub = p2_nminus1 * (seq_t)n - 1;
        sq -= sub;
    }

    /* multiply by 2^(n-1) * n again */
    seq_t result = p2_nminus1 * (seq_t)n * sq;
    return result;
}
