/*
 * A378923  Number of minimal edge cuts in the n-cycle complement and
 *          (n+1)-wheel complement graphs.
 * https://oeis.org/A378923
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) – 2*n – 1 for n ≥ 7. Special values: a(3)=0,
 *            a(4)=2, a(5)=10, a(6)=22
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the initial terms below the formula's validity range. */
    if (n == 3) return (seq_t)0;
    if (n == 4) return (seq_t)2;
    if (n == 5) return (seq_t)10;
    if (n == 6) return (seq_t)22;

    /* For n ≥ 7 the closed‑form holds. */
    seq_t pow   = pow2(n-1);       /* 2^(n‑1) */
    seq_t two_n = (seq_t)2 * (seq_t)n;
    return pow - two_n - (seq_t)1;
}
