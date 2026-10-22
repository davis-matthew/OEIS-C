/*
 * A340490  a(n) is the number of digits after the n-th digit of the
 *          Champernowne constant A033307 until the next appearance of that
 *          digit.
 * https://oeis.org/A340490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A033307(n - 1 + a(n)) = A033307(n - 1)
 * Range:     u8 n <= 9981, u16 n <= 9981, u32 n <= 9981, u64 n <= 9981,
 *            u128 n <= 9981, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine returns the number of digits after the n‑th Champernowne digit
   until the next occurrence of that same digit.  It scans the Champernowne
   sequence on demand, works for any idx_t ≥ 1, and uses only integer
   arithmetic. */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t pos = 1;               /* 1‑based position in the Champernowne string */
    idx_t i   = 1;               /* current integer whose digits are being output */
    uint8_t target = 0;          /* digit at position n */
    bool had_target = false;     /* became true once target has been recorded */

    for (;;) {
        /* Convert i to decimal digits in normal order into a small buffer. */
        idx_t tmp = i;
        uint8_t buf[20];
        idx_t len = 0;

        if (tmp == 0) {
            buf[0] = 0;
            len = 1;
        } else {
            while (tmp > 0) {
                buf[len++] = (uint8_t)(tmp % 10);
                tmp /= 10;
            }
            /* Reverse to obtain the digits in correct order. */
            for (idx_t k = 0; k < len / 2; ++k) {
                uint8_t t = buf[k];
                buf[k] = buf[len - 1 - k];
                buf[len - 1 - k] = t;
            }
        }

        /* Scan through the digits of i. */
        for (idx_t k = 0; k < len; ++k) {
            uint8_t d = buf[k];

            if (pos == n) {                 /* capture the target digit */
                target = d;
                had_target = true;
            }

            if (pos > n && had_target && d == target) {
                /* Distance from the n‑th digit to the next same digit. */
                return (seq_t)(pos - n);
            }
            ++pos;
        }
        ++i; /* proceed to the next integer */
    }
}
