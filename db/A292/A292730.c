/*
 * A292730  Numbers in which 0 outnumbers all other digits together.
 * https://oeis.org/A292730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth non‑negative integer whose decimal digit 0 occurs
 *            more times than any other digit combined (brute‑force scan)
 * Range:     u8 n <= 3, u16 n <= 237, u32 n <= 767, u64 n <= 767,
 *            u128 n <= 767, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)            /* safety for illegal offsets */
        return (seq_t)0;

    idx_t count = 0;
    idx_t value = 0;

    for (;;) {
        idx_t tmp = value;
        int zeros   = 0;
        int nonzeros = 0;

        if (tmp == 0) {
            zeros = 1;            /* the single digit 0 */
        } else {
            while (tmp > 0) {
                int d = tmp % 10;
                if (d == 0)
                    ++zeros;
                else
                    ++nonzeros;
                tmp /= 10;
            }
        }

        if (zeros > nonzeros) {
            ++count;
            if (count == n)
                return (seq_t)value;
        }
        ++value;
    }
}
