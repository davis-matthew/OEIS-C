/*
 * A381043  Centered pentagonal numbers which are squarefree semiprimes.
 * https://oeis.org/A381043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   count_prime_factors, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A381043: centered pentagonal numbers that are squarefree semiprimes.
   a(n) = (5k^2 + 5k + 2)/2   for the k-th centered pentagonal that is a
   squarefree semiprime (product of two distinct primes).  */
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/count_prime_factors.h"

/* Return the n-th centered pentagonal number that is a squarefree semiprime.
   The function works for any idx_t n >= 1. */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for offset 1, but safe */

    idx_t found = 0;                 /* how many terms found so far   */
    idx_t k = 1;                     /* index of current centered pentagonal */

    for (;;) {
        seq_t kk = (seq_t)k;
        seq_t pent = ((seq_t)5 * kk * kk + (seq_t)5 * kk + (seq_t)2) / (seq_t)2;

        /* test if pent is a squarefree semiprime */
        if (is_squarefree(pent) && count_prime_factors(pent) == 2) {
            ++found;
            if (found == n) return pent;
        }
        ++k;
    }
}
