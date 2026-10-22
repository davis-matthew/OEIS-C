/*
 * A389426  a(n) is the least k such that k*(n-k) + 1 is prime, or -1 if there
 *          is no such k.
 * https://oeis.org/A389426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least k ∈ [1, n‑1] with k·(n‑k)+1 prime; a(n) = –1 if
 *            none
 * Range:     i8 n <= 3509, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)                     /* no candidates when n = 1 (offset 1)   */
        return (seq_t)-1;

    for (idx_t k = 1; k < n; ++k) {
        uint64_t val = (uint64_t)k * (uint64_t)(n - k) + 1;
        if (is_prime(val))
            return (seq_t)k;       /* first k that yields a prime */
    }
    return (seq_t)-1;               /* no such k found */
}
