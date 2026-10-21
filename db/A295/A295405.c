/*
 * A295405  a(n) = 1 if n^2+1 is prime, 0 otherwise.
 * https://oeis.org/A295405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n^2+1 is prime, 0 otherwise
 * Range:     u8 n <= 15, u16 n <= 3235, u32 n <= 9054, u64 n <= 9054,
 *            u128 n <= 9054, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n) {
    /* compute n^2 + 1 using seq_t arithmetic */
    seq_t sq = (seq_t)n * (seq_t)n + 1;
    bool prime = isPrimeGeneric(sq);
    return (seq_t)(prime);
}
