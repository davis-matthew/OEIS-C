/*
 * A260857  Base-7 representation of a(n) is the concatenation of the base-7
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of base‑7 representations of
 *            1,2,…,n,n‑1,…,1 (as an integer whose base‑7 digits form that
 *            concatenation)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t result = 0;
    const unsigned base = 7;
    unsigned digits[32];
    unsigned count;

    /* Ascending part 1 … n */
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t tmp = (uint64_t)i;
        count = 0;
        do {
            digits[count++] = tmp % base;
            tmp /= base;
        } while (tmp != 0);
        for (int k = (int)count - 1; k >= 0; --k) {
            result = result * base + digits[k];
        }
    }

    /* Descending part n‑1 … 1 */
    if (n > 1) {
        for (idx_t i = n - 1; i > 0; --i) {
            uint64_t tmp = (uint64_t)i;
            count = 0;
            do {
                digits[count++] = tmp % base;
                tmp /= base;
            } while (tmp != 0);
            for (int k = (int)count - 1; k >= 0; --k) {
                result = result * base + digits[k];
            }
        }
    }

    return result;
}
