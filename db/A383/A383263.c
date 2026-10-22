/*
 * A383263  Union of prime powers (A246655) and numbers that are not
 *          squarefree (A013929).
 * https://oeis.org/A383263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer that is either a prime or not squarefree
 *            (i.e. divisible by a square)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the union of all prime powers (A246655) and all non‑squarefree numbers (A013929) */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 2;

    for (;; ++m) {
        if (is_prime(m)) {
            ++count;
            if (count == n) return (seq_t)m;
        } else if (!is_squarefree((seq_t)m)) {
            ++count;
            if (count == n) return (seq_t)m;
        }
    }
}
