/*
 * A400612  a(n) is the number of divisors d of n such that the last digit of
 *          d is equal to the first digit of n.
 * https://oeis.org/A400612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, new
 * Formula:   a(n) = number of divisors d of n such that the last digit of d
 *            equals the first digit of n
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* not used in OEIS offset 1 */
    /* determine the first digit of n */
    uint64_t first = n;
    while (first >= 10) first /= 10;
    /* count qualifying divisors */
    seq_t count = 0;
    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            if (d % 10 == first) count++;
            uint64_t d2 = n / d;
            if (d2 != d && d2 % 10 == first) count++;
        }
    }
    return count;
}
