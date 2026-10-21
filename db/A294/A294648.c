/*
 * A294648  Irregular triangle read by rows, representing a family of
 *          sequences L(n), for n=1, 2, 3, ... The sequence L(n) (i.e., the
 *          n-th row) is the ordinance of vectors of the n-dimensional Boolean
 *          cube (hypercube) {0,1}^n in accordance with their (Hamming)
 *          weights, where the lexicographic order is chosen...
 * https://oeis.org/A294648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 519, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description:
   a(n) is the element at position n in the “weight‑lexicographic” order
   of all Boolean vectors.  For each dimension r we list all 2^r binary
   numbers sorted first by Hamming weight (number of 1‑bits) and then
   by numeric value.  The global sequence is obtained by concatenating
   these rows for r = 1, 2, 3, … .
   The algorithm computes the row r, the position within that row,
   the corresponding Hamming weight k, and finally the k‑sized binary
   number at that rank using combinatorial unranking.  All operations
   are performed with the generic integer type `seq_t`.  */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Determine the row r such that 2^(r+1)-2 >= n.
       Equivalently, r = ceil(log2(n+2)) - 1. */
    idx_t t = n + 2;               /* n + 2 is always > 0 for n >= 1 */
    idx_t r = 0;
    idx_t pow = 1;
    while (pow < t) {
        pow <<= 1;
        r++;
    }
    r--;                          /* r is now the row number (>=1) */

    /* Position within the row: 0‑based */
    idx_t pos = n - ((idx_t)1 << r) + 1;

    /* Determine the Hamming weight k of the element at this position.
       Count the numbers of each weight until pos falls within the range. */
    idx_t weight = 0;
    idx_t remaining = pos;
    for (weight = 0; weight <= r; ++weight) {
        seq_t cnt = binom((idx_t)r, (idx_t)weight);
        if (remaining < (idx_t)cnt) {
            break;
        }
        remaining -= (idx_t)cnt;
    }
    idx_t k = weight;              /* desired weight */
    seq_t rank_in_weight = (seq_t)remaining;   /* 0‑based rank within weight group */

    /* Unrank: construct the binary number with weight k
       that is at rank `rank_in_weight` among all numbers with that weight
       sorted by numeric (lexicographic) order. */
    seq_t value = 0;
    idx_t bits_to_set = k;
    seq_t g = rank_in_weight;
    for (int i = (int)r - 1; i >= 0; --i) {
        seq_t cnt0 = binom((idx_t)i, (idx_t)bits_to_set); /* numbers starting with 0 at this bit */
        if (g < cnt0) {
            /* keep bit i = 0 */
            continue;
        } else {
            /* set bit i = 1 */
            value |= ((seq_t)1 << i);
            g -= cnt0;
            if (bits_to_set == 0) break;
            bits_to_set--;
        }
    }
    return value;
}
