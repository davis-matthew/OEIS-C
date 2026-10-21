/*
 * A276855  Beatty sequence for (3 + golden ratio).
 * https://oeis.org/A276855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(7 + sqrt(5))/2)
 * Range:     u8 n <= 3, u16 n <= 56, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the identity floor(n*sqrt(5)) = floor(sqrt(5*n*n)) */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t n_sq = (seq_t)n * (seq_t)n;            /* n^2        */
    seq_t sqrt5n = floor_sqrt((seq_t)5 * n_sq);  /* floor(n*sqrt5) */
    seq_t numer = (seq_t)7 * (seq_t)n + sqrt5n;   /* 7*n + sqrt5*n */
    return numer / (seq_t)2;                     /* floor(.../2)  */
}
