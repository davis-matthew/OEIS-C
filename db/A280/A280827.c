/*
 * A280827  a(n) = A076649(n) - A055642(n).
 * https://oeis.org/A280827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(n) = A076649(n) - A055642(n) Where A076649(n) = sum_{p^e||n} e
 *            * digits(p), digits(p) = number of decimal digits of prime p.
 *            And A055642(n) = digits(n), the number of decimal digits of n
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    /* Handle the only special case: n = 1 */
    if (n == 1) {
        seq_t digits_n = (seq_t)digit_count((uint64_t)1); /* = 1 */
        return (seq_t)0 - digits_n; /* -1 */
    }

    uint64_t temp = (uint64_t)n;
    seq_t digit_sum = 0;

    /* Factor out 2 separately */
    uint64_t count = 0;
    while ((temp & 1ULL) == 0) { /* temp % 2 == 0 */
        temp >>= 1;              /* temp /= 2 */
        count++;
    }
    if (count) {
        seq_t digits_of_2 = (seq_t)digit_count(2); /* = 1 */
        digit_sum += digits_of_2 * count;
    }

    /* Factor odd primes */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            count = 0;
            while (temp % p == 0) {
                temp /= p;
                count++;
            }
            seq_t digits_p = (seq_t)digit_count(p);
            digit_sum += digits_p * count;
        }
    }

    /* If anything is left, it is a prime > sqrt(original n) */
    if (temp > 1) {
        seq_t digits_temp = (seq_t)digit_count(temp);
        digit_sum += digits_temp; /* exponent 1 */
    }

    seq_t digits_n = (seq_t)digit_count((uint64_t)n);
    return digit_sum - digits_n;
}
