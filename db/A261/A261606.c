/*
 * A261606  a(n) = Fibonacci(n) mod 60.
 * https://oeis.org/A261606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Fibonacci(n) mod 60 Recurrence: a(0)=0, a(1)=1,
 *            a(n)=(a(n-1)+a(n-2)) mod 60
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n) {
    /* Handle the first two values directly */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Fibonacci modulo a fixed period (Pisano period for 60 is 120). */
    idx_t r = n % 120;

    /* The first two terms repeat every 120 steps; treat them specially. */
    if (r == 0) return 0;
    if (r == 1) return 1;

    /* Compute the value iteratively using only modular addition. */
    seq_t prev = 0;
    seq_t cur  = 1;
    for (size_t i = 2; i <= (size_t)r; ++i) {
        seq_t next = (prev + cur) % 60;
        prev = cur;
        cur  = next;
    }
    return cur;
}
