/*
 * A264994  Bijective base-4 reverse: a(0) = 0; for n >= 1, a(n) =
 *          A030103(A065883(n)) * A234957(n).
 * https://oeis.org/A264994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; for n > 0, a(4*n) = 4*a(n); otherwise a(n) is the
 *            integer obtained by reversing the base‑4 representation of n
 * Range:     u8 n <= 255, u16 n <= 8769, u32 n <= 8769, u64 n <= 8769,
 *            u128 n <= 8769, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* a(4*n) = 4*a(n) */
    if ((n & 3) == 0) return 4 * a(n >> 2);

    /* Reverse base‑4 digits of n. */
    seq_t r = 0;
    idx_t m = n;
    while (m != 0) {
        unsigned digit = m & 3;      /* digit = m % 4 */
        r = r * 4 + digit;           /* append digit as new least significant */
        m >>= 2;                     /* m = m / 4 */
    }
    return r;
}
