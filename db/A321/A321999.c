/*
 * A321999  Sum of digits of n minus the number of digits of n.
 * https://oeis.org/A321999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = A007953(n) – A004218(n+1) = sum_of_digits(n) –
 *            number_of_digits(n)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t temp = n;
    seq_t sum = 0;
    int count = 0;

    /* Zero has 0 digits, sum 0 → result 0 automatically */
    while (temp > 0) {
        sum += (seq_t)(temp % 10);
        temp /= 10;
        count++;
    }

    return sum - (seq_t)count;
}
