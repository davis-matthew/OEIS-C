/*
 * A275540  a(n) = n! + n!! - n - 1.
 * https://oeis.org/A275540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! + n!! - n - 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* compute n! using the provided factorial routine */
    seq_t fact = factorial(n);

    /* compute n!! (double factorial) */
    seq_t dfact = 1;
    for (idx_t i = n; i > 1; i -= 2) {
        dfact *= (seq_t)i;
    }

    /* a(n) = n! + n!! - n - 1 */
    seq_t result = fact + dfact - (seq_t)n - 1;
    return result;
}
