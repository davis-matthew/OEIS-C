/*
 * A309750  Number of letters in the English names of the months when the
 *          names are arranged in alphabetical order.
 * https://oeis.org/A309750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, word
 * Formula:   a(n) = A005589(A260249(n))
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const uint8_t month_perm[12] = {4, 8, 12, 2, 1, 7, 6, 3, 5, 11, 10, 9};
    static const uint8_t month_letter[13] = {0, 3, 3, 5, 4, 4, 3, 5, 5, 4, 3, 6, 6};

    idx_t idx = (n - 1) % 12;
    uint8_t month = month_perm[idx];
    return (seq_t)month_letter[month];
}
