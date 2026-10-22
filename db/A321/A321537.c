/*
 * A321537  Write n in base 10, shorten all the runs of successive identical
 *          digits by 1.
 * https://oeis.org/A321537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal number obtained by reducing every run of
 *            identical consecutive digits of the decimal representation of n
 *            by one. Each run of length L is replaced by L‑1 copies of that
 *            digit (runs of length 1 are removed). The resulting string is
 *            interpreted as a decimal integer (empty -> 0)
 * Range:     u8 n <= 3332, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* "0" becomes empty -> 0 */
        return 0;

    /* Extract decimal digits of n into an array (least significant first). */
    uint8_t digits[21];
    idx_t pos = 0;
    idx_t tmp = n;
    while (tmp) {
        digits[pos++] = tmp % 10;
        tmp /= 10;
    }

    seq_t result = 0;
    int  prev_digit = -1;     /* placeholder before first digit */
    idx_t run_len = 0;

    /* Process digits from most significant to least. */
    for (idx_t di = pos; di-- > 0;) {
        int d = digits[di];

        if (run_len == 0) {  /* start of a new run */
            prev_digit = d;
            run_len = 1;
        } else if (d == prev_digit) { /* continue the run */
            ++run_len;
        } else {                /* run ended, emit run_len-1 copies */
            for (idx_t k = 0; k < run_len - 1; ++k)
                result = result * 10 + (seq_t)prev_digit;
            prev_digit = d;
            run_len = 1;
        }
    }
    /* Final run */
    for (idx_t k = 0; k < run_len - 1; ++k)
        result = result * 10 + (seq_t)prev_digit;

    return result;
}
