/*
 * A360015  Numbers whose exponent of 2 in their canonical prime factorization
 *          is equal to the maximal exponent.
 * https://oeis.org/A360015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 115, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_good_64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A360015: numbers whose exponent of 2 in the prime factorisation
 * is equal to the maximum exponent among all primes. 
 * The sequence is built by testing integers in ascending order and selecting
 * those that satisfy the condition.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_good_64.h"

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 1ULL) return (seq_t)1;          /* first term is 1 */

    idx_t count = 1;                         /* 1 already counted */
    uint64_t candidate = 1;

    while (true) {
        candidate++;                         /* test next integer */
        if (is_good_64(candidate)) {
            count++;
            if (count == n) return (seq_t)candidate;
        }
    }
}
