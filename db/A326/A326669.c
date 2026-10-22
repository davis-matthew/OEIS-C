/*
 * A326669  Numbers k such that the average position of the ones in the binary
 *          expansion of k is an integer.
 * https://oeis.org/A326669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer k such that the average * of
 *            the positions of 1-bits in the binary representation of k is an
 *            integer
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 0;
    while (1) {
        k++;
        seq_t tmp = k;
        idx_t m = 0;
        seq_t sum = 0;
        idx_t pos = 0;
        while (tmp != 0) {
            if (tmp & 1) {
                m++;
                sum += (seq_t)pos;
            }
            tmp >>= 1;
            pos++;
        }
        if (m != 0 && (sum % m == 0)) {
            count++;
            if (count == n)
                return k;
        }
    }
}
