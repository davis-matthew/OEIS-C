/*
 * A319652  Write n in 4-ary, sort digits into increasing order.
 * https://oeis.org/A319652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the number obtained by writing n in base‑4 and sorting
 *            its * digits in increasing order
 * Range:     u8 n <= 340, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Collect the base‑4 digits of n (LSB first). */
    idx_t temp = n;
    int digits[64];
    size_t count = 0;

    if (temp == 0) {
        digits[0] = 0;
        count = 1;
    } else {
        while (temp) {
            digits[count++] = (int)(temp & 3); /* temp % 4 */
            temp >>= 2;                       /* temp /= 4 */
        }
    }

    /* Sort the digits in ascending order (insertion sort). */
    for (size_t i = 1; i < count; ++i) {
        int key = digits[i];
        size_t j = i;
        while (j > 0 && digits[j - 1] > key) {
            digits[j] = digits[j - 1];
            --j;
        }
        digits[j] = key;
    }

    /* Rebuild the number from the sorted digits, treating digits[0] as the most
     * significant digit. */
    seq_t res = 0;
    for (size_t i = 0; i < count; ++i) {
        res = res * 4 + (seq_t)digits[i];
    }
    return res;
}
