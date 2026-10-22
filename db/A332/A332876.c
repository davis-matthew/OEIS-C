/*
 * A332876  a(n) is the smallest positive multiple of n whose decimal
 *          expansion includes a digit (other than a trailing zero) whose
 *          removal yields a proper multiple of n.
 * https://oeis.org/A332876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search for the smallest positive multiple of n whose
 * decimal expansion contains a digit (other than a trailing zero)
 * whose removal yields a proper multiple of n (i.e. a multiple of n
 * different from n). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n=0, safety guard */
    const seq_t n_seq = (seq_t)n;

    /* Buffer big enough for decimal digits of any seq_t value. */
    uint8_t digits[128];

    for (idx_t k = 1;; ++k) {
        const seq_t val = n_seq * (seq_t)k;

        /* Extract decimal digits of val (least significant first). */
        size_t len = 0;
        seq_t tmp = val;
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
        if (len == 0) { /* val == 0 never occurs, but keep for safety */
            digits[len++] = 0;
        }

        /* Reverse digits to most significant first. */
        for (size_t i = 0, j = len - 1; i < j; ++i, --j) {
            uint8_t t   = digits[i];
            digits[i]   = digits[j];
            digits[j]   = t;
        }

        /* Find start index of the trailing zero block.  Digits from
         * that index to the end are all zeros. */
        size_t tz_start = len;           /* default if no trailing zeros */
        if (len > 1) {
            size_t i = len - 1;
            while (i > 0 && digits[i] == 0) {
                --i;
            }
            tz_start = i + 1;
        }

        /* Test all removable digits that are not part of the trailing
         * zero block. */
        for (size_t pos = 0; pos < len; ++pos) {
            /* Skip any digit that lies in the trailing zero block. */
            if (pos >= tz_start) continue;

            /* Build number m obtained by removing digits[pos]. */
            seq_t m = 0;
            for (size_t i = 0; i < len; ++i) {
                if (i == pos) continue;
                m = m * 10 + (seq_t)digits[i];
            }

            if (m == 0)            /* must be positive */
                continue;
            if (m == n_seq)        /* proper multiple required */
                continue;
            if ((m % n_seq) == 0)  /* valid candidate found */
                return val;
        }
    }
}
