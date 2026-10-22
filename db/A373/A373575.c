/*
 * A373575  Numbers k such that k and k-1 both have at least two distinct
 *          prime factors. First element of the n-th maximal antirun of
 *          non-prime-powers.
 * https://oeis.org/A373575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n-th integer k (k ≥ 1) for which
 * both k and k‑1 have at least two distinct prime factors.
 * The first term (k = 1) is handled as a special case.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Offset is 1; n < 1 is undefined */
    if (n == 1) return 1;          /* The sequence starts with 1 */

    idx_t k = 2;                   /* Start checking from k = 2 */
    idx_t found = 1;               /* We already counted the term 1 */

    for (;; ++k) {
        /* Count distinct prime factors of k and k-1 */
        if ((count_distinct_primes(k) >= 2) &&
            (count_distinct_primes(k - 1) >= 2)) {
            ++found;
            if (found == n) return (seq_t)k;
        }
    }
}
