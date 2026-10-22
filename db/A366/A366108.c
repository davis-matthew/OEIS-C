/*
 * A366108  a(n) = floor(binomial(n-1, floor((n-1)/2))/2).
 * https://oeis.org/A366108
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(binomial(n-1, floor((n-1)/2)) / 2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* Offset 3: the sequence is defined for n >= 3. For n < 3
       we return 0, though any value is acceptable because the
       specification says behaviour below the offset does not matter.  */
    if (n < 3) return (seq_t)0;

    idx_t k = (n - 1) / 2;           /* floor((n-1)/2) */
    seq_t binom = (seq_t)1;

    /* Compute the binomial coefficient multiplicatively.
       binom(n-1, k) = ∏_{i=1}^{k} (n-1 - k + i) / i   */
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numerator = (seq_t)(n - 1 - k + i);
        binom = binom * numerator;
        binom = binom / (seq_t)i;
    }

    /* Divide by 2 and truncate (floor).  */
    return binom / (seq_t)2;
}
