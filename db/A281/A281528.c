/*
 * A281528  a(n) = least numerator k such that the proper fraction k/n needs
 *          three or more terms as a signed sum of distinct unit fraction, or
 *          0 if no such numerator exists.
 * https://oeis.org/A281528
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least numerator k (1 ≤ k < n) such that the proper
 *            fraction k/n cannot be expressed as a sum of at most two
 *            distinct unit fractions (with signs). Returns 0 if every 1 ≤ k <
 *            n has such a 2‑term representation
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   two_term_representation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>   /* for bool */
#include "../utils/two_term_representation.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* out of defined range – safe default */

    for (idx_t k = 1; k < n; ++k) {
        if (two_term_representation(n, k)) continue;
        return (seq_t)k;          /* first k without 2‑term representation */
    }
    return 0;                     /* all k have a 2‑term representation */
}
