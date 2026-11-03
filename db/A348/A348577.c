/*
 * A348577  Positive integers that are not the perimeter of any integer-sided
 *          right triangle.
 * https://oeis.org/A348577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer that is NOT the * perimeter of
 *            any integer‑sided right triangle. * * (The OEIS entry A348577
 *            lists such numbers. The function below * computes the nth term
 *            on demand, without any pre‑computed table.)
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 8831 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"

/* Mark all perimeters <= limit that come from integer right triangles.
 * present[i] == 1  <=>  i is a perimeter; 0 otherwise. */
static void mark_perimeters(uint64_t limit, uint8_t *present)
{
    /* Euclid's formula: primitive perimeter = 2*m*(m+n) with
       coprime m,n of opposite parity.  All multiples of a primitive
       perimeter are also perimeters. */
    for (uint64_t m = 2; 2ULL * m * (m + 1) <= limit; ++m) {
        for (uint64_t n = 1; n < m; ++n) {
            if (((m - n) & 1U) == 0)            /* same parity – skip */
                continue;
            if (gcd_u64(m, n) != 1)            /* not coprime – skip */
                continue;

            uint64_t p0 = 2ULL * m * (m + n);   /* primitive perimeter */
            if (p0 > limit)
                break;                          /* larger n → larger p0 */

            /* Mark all multiples of this primitive perimeter. */
            for (uint64_t p = p0; p <= limit; p += p0)
                present[p] = 1;
        }
    }
}

/* Greatest common divisor for 64‑bit values. */

seq_t a(idx_t n)
{
    /* Ensure the argument is positive.  (The sequence is defined for n>=1.) */
    if (n == 0)
        return (seq_t)0;

    /* We'll keep trying larger limits until we get enough missing numbers. */
    uint64_t limit = n * 3ull + 100ull;   /* rough starting guess */
    while (1) {
        /* Allocate a boolean array indexed from 1 to 'limit'. */
        uint8_t *present = (uint8_t*)calloc((size_t)(limit + 1), sizeof(uint8_t));
        if (!present)
            return (seq_t)0;  /* allocation failure – unlikely for the expected limits */

        /* Generate all perimeters up to the current limit. */
        mark_perimeters(limit, present);

        /* Scan to find the n‑th missing number. */
        uint64_t count = 0;
        uint64_t answer = 0;
        for (uint64_t i = 1; i <= limit; ++i) {
            if (!present[i]) {
                ++count;
                if (count == (uint64_t)n) {
                    answer = i;
                    break;
                }
            }
        }

        free(present);

        if (count >= (uint64_t)n)
            return (seq_t)answer;   /* success */

        /* Not enough missing numbers – increase the limit and retry. */
        /* Double the limit to keep the number of iterations O(log n). */
        limit = limit + limit;  /* same as limit *= 2 but avoids overflow on small values */
    }
}
