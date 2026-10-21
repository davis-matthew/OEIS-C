/*
 * A271743  Number of set partitions of [n] such that 4 is the largest element
 *          of the last block.
 * https://oeis.org/A271743
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 + 2^(n-2) + 3^(n-4) for n > 4; a(4) = 10
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 44, u128 n <= 84,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Compute a(n) for the OEIS A271743 sequence.
 * Index n is 0‑based unsigned, but the sequence starts at n = 4.
 * For n < 4 the function returns 0 (not used by the tests). */
seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* undefined for n < 4 */

    if (n == 4) return (seq_t)10; /* first term */

    /* Use the explicit formula given in the OEIS entry.
     * big_pow computes base^exp using the provided utilities. */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n - 2));
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n - 4));

    return (seq_t)4 + pow2 + pow3;
}
