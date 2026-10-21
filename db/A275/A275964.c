/*
 * A275964  Total number of nonzero digits with multiple occurrences in
 *          factorial base representation of n (counted with multiplicity):
 *          a(n) = A275812(A275735(n)).
 * https://oeis.org/A275964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A275812(A275735(n))
 * Range:     u8 n <= 9298, u16 n <= 9298, u32 n <= 9298, u64 n <= 9298,
 *            u128 n <= 9298, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t temp = n;
    /* The digit values in factorial base for n <= 2^64-1 never exceed 20.
       Use a small fixed array for their counts. */
    unsigned int freq[32] = {0};
    unsigned int max_digit = 0;

    for (idx_t i = 1; temp > 0; ++i) {
        idx_t digit = temp % (i + 1);
        temp /= (i + 1);
        if (digit != 0) {
            freq[(unsigned int)digit] += 1;
            if (digit > max_digit) max_digit = (unsigned int)digit;
        }
    }

    seq_t result = 0;
    for (unsigned int d = 1; d <= max_digit; ++d) {
        if (freq[d] > 1) {
            result += (seq_t)freq[d];
        }
    }

    return result;
}
