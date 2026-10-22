/*
 * A364379  Greedy Jacobsthal-Niven numbers: numbers that are divisible by the
 *          sum of the digits in their representation in Jacobsthal greedy
 *          base (A265747).
 * https://oeis.org/A364379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacob_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Greedy Jacobsthal‑Niven numbers by scanning integers
   and checking that the number is divisible by the sum of its
   digits in greedy Jacobsthal base (A265747). */

#include <stddef.h>  /* size_t */
#include "../utils/jacob_digit_sum.h"

seq_t
a(idx_t n)
{
    idx_t found = 0;
    seq_t k = 1;
    while (1) {
        seq_t s = jacob_digit_sum(k);
        if (s != 0 && (k % s) == 0) {
            ++found;
            if (found == n) return k;
        }
        ++k;
    }
}
