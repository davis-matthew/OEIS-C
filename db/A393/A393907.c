/*
 * A393907  Number of planar embedding isomorphism classes of the n-Hanoi
 *          graph.
 * https://oeis.org/A393907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8^(n-1) + 2*2^(n-1) + 3*4^(n-1))/12 for n > 1, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"   /* provides seq_t pow_seq(seq_t base, int exp) */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Small‑integer branch – avoids overflow when seq_t is a 64‑bit type. */
    if (sizeof(seq_t) <= 8 && n <= 43) /* (n-1) ≤ 42 → exponents ≤ 42, 2·42=84, 3·42=126 */
    {
        idx_t e   = (idx_t)(n - 1);      /* e ≤ 42 */
        unsigned __int128 p2 = (unsigned __int128)1 << e;       /* 2^(n-1)   */
        unsigned __int128 p4 = (unsigned __int128)1 << (2 * e); /* 4^(n-1)   */
        unsigned __int128 p8 = (unsigned __int128)1 << (3 * e); /* 8^(n-1)   */

        unsigned __int128 sum = p8 + (p2 * (unsigned __int128)2) + (p4 * (unsigned __int128)3);
        return (seq_t)(sum / (unsigned __int128)12);
    }

    /* General‑big‑integer branch – works for all seq_t types (automatic). */
    int e = (int)(n - 1);           /* n fits in int for n ≤ 10000 */
    seq_t p2 = pow_seq((seq_t)2, e); /* 2^(n-1)   */
    seq_t p4 = pow_seq((seq_t)4, e); /* 4^(n-1)   */
    seq_t p8 = pow_seq((seq_t)8, e); /* 8^(n-1)   */

    seq_t sum = p8 + (p2 * (seq_t)2) + (p4 * (seq_t)3);
    return sum / (seq_t)12;
}
