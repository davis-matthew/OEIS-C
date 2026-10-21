/*
 * A262458  Number of possible conjunction tables in the n-valued logics.
 * https://oeis.org/A262458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)^(n^2-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Computes the number of possible conjunction tables for the n‑valued logic.
 * The sequence is a(n) = (n-1)^(n^2-1), with the convention 0^0 = 1. */
seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)1;   /* a(1) = 1, and 0^0 is defined as 1 */
    seq_t base = (seq_t)(n - 1);          /* n-1 */
    seq_t exp  = (seq_t)n * (seq_t)n - 1; /* n^2-1 */
    return big_pow(base, exp);
}
