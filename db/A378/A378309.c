/*
 * A378309  a(n) = A010051(n) + A001222(n).
 * https://oeis.org/A378309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A010051(n) + A001222(n), i.e. (1 if n is prime else 0) +
 *            bigomega(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9643 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t A001222_a(idx_t n) {
    if (n == 1) return 0;
    seq_t count = 0;
    idx_t temp = n;
    while (temp % 2 == 0) {
        count++;
        temp /= 2;
    }
    for (idx_t i = 3; i <= temp / i; i += 2) {
        while (temp % i == 0) {
            count++;
            temp /= i;
        }
    }
    if (temp > 1) {
        count++;
    }
    return count;
}

seq_t a(idx_t n) {
    seq_t omega = A001222_a(n);
    /* n is prime iff n > 1 and bigomega(n) == 1 */
    if (n > 1 && omega == 1) return (seq_t)2;
    return omega;
}
