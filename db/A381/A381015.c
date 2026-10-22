/*
 * A381015  a(n) = n + (number of trailing 0's of n).
 * https://oeis.org/A381015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + A122840(n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t temp = n;
    while ((temp % (idx_t)10) == 0 && temp > 0) {
        count++;
        temp /= (idx_t)10;
    }
    return (seq_t)n + (seq_t)count;
}
