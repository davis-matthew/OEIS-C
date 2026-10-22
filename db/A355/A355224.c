/*
 * A355224  The k-th rightmost digit of a(n) is the greatest of the k
 *          rightmost digits of n.
 * https://oeis.org/A355224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes OEIS A355224: a(n) where the k-th rightmost digit of a(n) is
 * the greatest of the k rightmost digits of n.
 * The algorithm scans the digits of n from least significant to most,
 * keeping the running maximum and appending it to the result.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t result = 0;
    seq_t pow10   = 1;
    seq_t current = 0;
    idx_t temp = n;
    while (temp > 0) {
        unsigned digit = temp % 10;     /* 0 <= digit <= 9 */
        if (digit > current) current = digit;
        result += current * pow10;
        pow10 *= 10;
        temp   /= 10;
    }
    return result;
}
