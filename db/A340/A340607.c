/*
 * A340607  Number of factorizations of n into an odd number of factors > 1,
 *          the greatest of which is odd.
 * https://oeis.org/A340607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of factorizations of n into an odd number of
 *            factors >1 where the greatest factor is odd
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 124 terms through n = 9965 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Recursive helper that counts factorizations of 'rem'.
   'min_factor' ensures non‑decreasing order of factors,
   'parity' counts how many factors have already been chosen,
   'max_factor' is the last factor chosen (i.e. the current maximum),
   'cnt' accumulates the total number of valid factorizations. */
static void _factor_rec(uint64_t rem, uint64_t min_factor,
                        int parity, uint64_t max_factor, seq_t *cnt)
{
    if (rem == 1) {
        /* Finished a factorization.  Count it only if it has an odd length
           and its maximum factor is odd. */
        if ((parity & 1) && (max_factor & 1)) {
            (*cnt) += 1;
        }
        return;
    }

    /* Consider the factor equal to the remaining product (single‑factor
       continuation of the current factorization). */
    if (rem >= min_factor) {
        _factor_rec(1, rem, parity + 1, rem, cnt);
    }

    /* Enumerate proper divisors of 'rem' that are >= min_factor.
       For each such divisor 'f', recurse on the quotient rem/f. */
    for (uint64_t f = min_factor; f * f <= rem; ++f) {
        if (rem % f == 0) {
            _factor_rec(rem / f, f, parity + 1, f, cnt);
            uint64_t g = rem / f;
            if (g != f && g >= min_factor) {
                _factor_rec(rem / g, g, parity + 1, g, cnt);
            }
        }
    }
}

seq_t a(idx_t n)
{
    /* Sequence starts with a(1)=0; for n < 2 no valid factorization exists. */
    if (n < 2) {
        return 0;
    }

    seq_t count = 0;
    _factor_rec((uint64_t)n, 2, 0, 0, &count);
    return count;
}
