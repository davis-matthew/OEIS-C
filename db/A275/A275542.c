/*
 * A275542  The digits of the integers with the nonprimes removed.
 * https://oeis.org/A275542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7981, u16 n <= 7981, u32 n <= 7981, u64 n <= 7981,
 *            u128 n <= 7981, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: digits of positive integers, keeping only prime digits (2,3,5,7).
   Computed by enumerating integers and scanning each number’s digits in
   left‑to‑right order. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* the sequence is 1-indexed; 0 is out of range */
        return 0;

    idx_t count = 0;

    /* The maximum number of decimal digits for an idx_t value is 20
       (for 64‑bit unsigned integers). */
    uint8_t buf[20];

    for (idx_t num = 1; ; ++num) {
        /* Extract digits of num into buf in reverse order */
        idx_t tmp = num;
        int len = 0;
        while (tmp > 0) {
            buf[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
        /* Scan digits from most significant to least significant */
        for (int k = len - 1; k >= 0; --k) {
            uint8_t d = buf[k];
            if (d == 2 || d == 3 || d == 5 || d == 7) {
                ++count;
                if (count == n) {
                    return (seq_t)d;
                }
            }
        }
    }
}
