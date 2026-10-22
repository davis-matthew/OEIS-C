/*
 * A375267  Number of iterations of the A375265 map to reach 1 starting from
 *          n, or -1 if 1 is never reached.
 * https://oeis.org/A375267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of iterations of the map f to reach 1, where f is
 *            defined by f(1) = 1 f(n) = n/2 if n is even f(n) = n/3 if n>1,
 *            odd, and n≡0 (mod 3) f(n) = 3*n + 1 if n>1, odd, and n≢0 (mod 3)
 *            The iteration count a(1) is 0
 * Range:     u8 n <= 6942, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static inline idx_t f(idx_t n) {
    if ((n & 1) == 0)          /* even */
        return n >> 1;
    if (n % 3 == 0)            /* odd and divisible by 3 */
        return n / 3;
    return 3 * n + 1;          /* odd and not divisible by 3 */
}

seq_t a(idx_t n) {
    seq_t steps = 0;
    while (n != 1) {
        n = f(n);
        steps = steps + (seq_t)1;
    }
    return steps;
}
