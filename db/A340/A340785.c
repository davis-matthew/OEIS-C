/*
 * A340785  Number of factorizations of 2n into even factors > 1.
 * https://oeis.org/A340785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of 2*n into even
 *            factors > 1
 * Range:     u8 n <= 5759, u16 n <= 9909, u32 n <= 9909, u64 n <= 9909,
 *            u128 n <= 9909, big any n
 * Helpers:   rec_factorization
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rec_factorization.h"

seq_t a(idx_t n) {
    idx_t prod = 2 * n;                /* compute 2*n  */
    return rec_factorization(prod, 2); /* start recursion with factor 2 */
}
