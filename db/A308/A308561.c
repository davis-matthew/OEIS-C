/*
 * A308561  Numbers with no 0 digit that are divisible by the sum of any two
 *          of their digits at distinct positions.
 * https://oeis.org/A308561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer with no digit 0 that is
 *            divisible by the sum of any two of its digits in distinct
 *            positions. Computation is performed by a brute‑force search
 * Range:     u8 n <= 24, u16 n <= 47, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Candidate numbers are stored in an unsigned 64‑bit type so that
       they can grow beyond the width of seq_t (when seq_t is smaller
       such as uint8_t).  The function returns the value cast to seq_t. */
    idx_t cur = 1;
    idx_t count = 0;

    /* Buffer for decimal digits; 64‑bit integers have at most 20 digits,
       but 128‑bit arbitrary precision may have up to 39 digits. */
    uint8_t digits[40];

    while (true) {
        /* Extract digits of cur, checking for any zero digit. */
        idx_t tmp = cur;
        size_t len = 0;
        bool has_zero = false;

        while (tmp > 0) {
            uint8_t d = (uint8_t)(tmp % 10);
            if (d == 0) {
                has_zero = true;
            }
            digits[len++] = d;
            tmp /= 10;
        }

        /* Numbers with a zero digit are skipped. */
        if (!has_zero) {
            bool ok = true;

            if (len > 1) {  /* single‑digit numbers are always valid */
                for (size_t i = 0; i + 1 < len && ok; ++i) {
                    for (size_t j = i + 1; j < len && ok; ++j) {
                        idx_t sum = (idx_t)(digits[i] + digits[j]); /* 2–18 */
                        if (cur % sum != 0) {
                            ok = false;
                        }
                    }
                }
            }

            if (ok) {
                ++count;
                if (count == n) {
                    return (seq_t)cur;
                }
            }
        }

        ++cur;
    }
}
