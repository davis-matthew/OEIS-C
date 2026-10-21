/*
 * A244642  Number of nonzero cells at n-th stage in some 2D reversible
 *          second-order cellular automata (see comments for precise
 *          definition).
 * https://oeis.org/A244642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A244642: Number of nonzero cells at n‑th stage in the
 * 2D reversible second‑order cellular automaton.
 *
 * Recurrence (from the OEIS entry):
 *   a(0) = 1
 *   For n > 0 let 2^k be the greatest power of two ≤ n and j = n – 2^k:
 *     a(n) = 4 * a(j) + a(2^k – j – 1)
 *
 * The implementation uses this exact recurrence with pure recursion,
 * which is fast enough for n up to at least 10 000 and works for all
 * supported seq_t types.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Find the greatest power of two ≤ n without overflow. */
    idx_t pow = 1;
    while (pow <= n / 2)
        pow <<= 1;

    idx_t j = n - pow;          /* 0 ≤ j < pow */
    seq_t part1 = a(j);         /* a(j)   */
    seq_t part2 = a(pow - j - 1); /* a(2^k – j – 1) */
    return 4 * part1 + part2;
}
