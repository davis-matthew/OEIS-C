/*
 * A353651  Unique monotonic sequence of positive integers satisfying a(a(n))
 *          = k*(n-1) + 3, where k = 4.
 * https://oeis.org/A353651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(1) = 2 * For i ≥ 0, let p = 4ⁱ. * * If (2p + 1)/3 < n ≤ (5p
 *            + 1)/3 a(n) = n + p * If (5p + 1)/3 < n ≤ (8p + 1)/3 a(n) = 4·(n
 *            – p) – 1 * * This implements the unique monotonic sequence
 *            satisfying a(a(n)) = 4·(n‑1)+3
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1u)
        return (seq_t)2;

    seq_t sn = (seq_t)n;   // work in seq_t for exactness
    seq_t p  = (seq_t)1;   // p = 4ⁱ, start with i = 0

    /* Find i such that n ≤ (8·p + 1)/3  (upper bound of the i‑th block) */
    while (1) {
        seq_t upper = ( (seq_t)8 * p + (seq_t)1 ) / (seq_t)3;
        if (sn <= upper)
            break;
        p = p * (seq_t)4;
    }

    /* Decide which side of the current block n lies in */
    seq_t r1 = ( (seq_t)5 * p + (seq_t)1 ) / (seq_t)3;  /* (5p+1)/3 */
    if (sn <= r1)
        return sn + p;                      /* First part of the block */
    else
        return (seq_t)4 * (sn - p) - (seq_t)1;  /* Second part of the block */
}
