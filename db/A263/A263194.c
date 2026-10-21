/*
 * A263194  4-digit numbers (with leading zeros supplied where necessary) in
 *          which the sum of the number consisting of the first two digits and
 *          the number consisting of the last two digits equals the number
 *          consisting of the middle two digits.
 * https://oeis.org/A263194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   a(n) is the nth smallest 4‑digit number (with leading zeros)
 *            whose first two digits (ab) plus the last two digits (cd) equal
 *            the middle two digits (bc), i.e. 10*a + b + 10*c + d = 10*b + c.
 *            The value a(1) = 0
 * Range:     u8 n <= 5, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t k = 0;
    for (idx_t i = 0; i < 10000; ++i) {
        seq_t v = (seq_t)i;
        seq_t t   = v / 1000;               // thousands digit
        seq_t h   = (v / 100) % 10;         // hundreds digit
        seq_t te  = (v / 10) % 10;          // tens digit
        seq_t o   = v % 10;                 // ones digit
        seq_t ab  = t * 10 + h;             // first two digits
        seq_t bc  = h * 10 + te;            // middle two digits
        seq_t cd  = te * 10 + o;            // last two digits
        if (ab + cd == bc) {
            ++k;
            if (k == n) return v;
        }
    }
    return 0; /* n exceeds the number of terms (55) */
}
