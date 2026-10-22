/*
 * A353490  The largest proper divisor of n, reduced modulo 4, with a(1) = 1.
 * https://oeis.org/A353490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (largest proper divisor of n) mod 4, with a(1) = 1
 * Range:     u8 n <= 9715, u16 n <= 9715, u32 n <= 9715, u64 n <= 9715,
 *            u128 n <= 9715, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t spf = 0;

    /* Find the smallest prime factor by simple trial division. */
    for (uint64_t i = 2; i * i <= nn; ++i) {
        if (nn % i == 0) {
            spf = i;
            break;
        }
    }
    if (spf == 0) spf = nn;       /* n is prime */

    uint64_t largest_prop = nn / spf;
    return (seq_t)(largest_prop % 4);
}
