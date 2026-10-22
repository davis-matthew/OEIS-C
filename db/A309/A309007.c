/*
 * A309007  Largest k such that n^k has distinct digits in base 10 (for n>1).
 * https://oeis.org/A309007
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest k such that n^k has all distinct decimal digits
 *            (a(n) = 0 for n <= 1 or n > 9876543210)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   digit_len, has_unique_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/has_unique_digits.h"

seq_t a(idx_t n)
{
    /* Undefined for n <= 1; defined as 0 for n > 9876543210. */
    if (n <= 1)
        return 0;

    const seq_t MAX_N = (seq_t)9876543210ULL;
    if ((seq_t)n > MAX_N)
        return 0;

    seq_t power = (seq_t)n;   /* n^1 */
    seq_t best_k = 0;

    for (seq_t k = 1; digit_len(power) <= 10; ++k) {
        if (has_unique_digits(power))
            best_k = k;
        power *= (seq_t)n;   /* advance to n^(k+1) */
    }

    return best_k;
}
