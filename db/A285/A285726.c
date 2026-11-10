/*
 * A285726  a(1) = a(2) = 0; for n > 2, a(n) = A252736(n) - (1-A000035(n)).
 * https://oeis.org/A285726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 0; for n > 2, a(n) = (bigomega(n) - 1) - (1 - n
 *            mod 2)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 136 terms through n = 7045 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;

    idx_t t = n;
    unsigned int cnt = 0;

    while ((t & 1u) == 0u) {
        ++cnt;
        t >>= 1;
    }
    for (idx_t p = 3; p <= t / p; p += 2) {
        while (t % p == 0) {
            ++cnt;
            t /= p;
        }
    }
    if (t > 1)
        ++cnt;

    unsigned int r = cnt - 1;          /* A252736(n) */
    if ((n & 1u) == 0u)
        r -= 1;                        /* subtract 1 - A000035(n) */
    return (seq_t)r;
}
