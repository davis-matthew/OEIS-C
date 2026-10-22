/*
 * A384517  Nonsquarefree numbers that are squarefree numbers raised to an
 *          even power.
 * https://oeis.org/A384517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A062770(n)^2 = A072774(n+1)^2 (narratively: the square of
 *            the n‑th integer whose prime factor exponents are all equal)
 * Range:     u8 n <= 13, u16 n <= 176, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   equal_exponents_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/equal_exponents_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 safely */
    idx_t count = 0;
    uint64_t cand = 2;             /* start from n = 2 */
    seq_t result = 0;

    while (count < n) {
        if (equal_exponents_uint64(cand)) {
            result = (seq_t)cand * (seq_t)cand;
            ++count;
        }
        ++cand;
    }
    return result;
}
