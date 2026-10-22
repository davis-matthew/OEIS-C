/*
 * A376131  Total number of times all nodes fire in a chip-firing game
 *          starting with 2n chips at the root on an infinite binary tree with
 *          a loop at the root.
 * https://oeis.org/A376131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..m-1} ((k-1)*2^k + 1) * (b(k)+1), where m =
 *            floor(log2(2*n+1)) and b(k) are the bits of 2*n+1
 * Range:     u8 n <= 42, u16 n <= 3583, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* value 2*n+1 as a 128‑bit unsigned integer to avoid overflow */
    unsigned __int128 val = ((unsigned __int128)n << 1) | 1;

    /* compute m = floor(log2(val)) */
    int m = 0;
    while ((val >> (m + 1)) != 0) m++;

    seq_t sum = 0;

    for (int k = 1; k <= m - 1; ++k) {
        /* ((k-1)*2^k + 1) */
        seq_t coeff = ((seq_t)(k - 1) * pow2(k)) + 1;

        /* bit b(k) of val (0 or 1) */
        unsigned int bit = (unsigned int)((val >> k) & 1ULL);

        /* add coeff * (b(k)+1) to the sum */
        sum += coeff * ((seq_t)(bit + 1));
    }

    return sum;
}
