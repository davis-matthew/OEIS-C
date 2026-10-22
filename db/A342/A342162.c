/*
 * A342162  a(n) = difference between the starting positions of the first two
 *          occurrences of n in the Champernowne string (cf. A033307), where
 *          the first n is not required to be at its natural position.
 * https://oeis.org/A342162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A342162: a(n) = difference between the starting positions of
   the first two occurrences of n in the Champernowne string. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert n to its decimal digit array (most significant first). */
    int pat[20];
    int L = 0;
    if (n == 0) {
        pat[0] = 0;
        L = 1;
    } else {
        idx_t t = n;
        while (t > 0) {
            pat[L++] = t % 10;
            t /= 10;
        }
        for (int i = 0; i < L / 2; i++) {
            int tmp = pat[i];
            pat[i] = pat[L - 1 - i];
            pat[L - 1 - i] = tmp;
        }
    }

    int win[20];
    int wlen = 0;
    seq_t first = 0, second = 0;
    bool foundFirst = false;

    idx_t pos = 0; /* number of digits processed so far */

    for (idx_t num = 0;; ++num) {
        /* Convert num to decimal digit array (most significant first). */
        idx_t t = num;
        char buf[32];
        int len = 0;
        if (t == 0) {
            buf[0] = '0';
            len = 1;
        } else {
            while (t > 0) {
                buf[len++] = '0' + (t % 10);
                t /= 10;
            }
            /* Reverse to correct order */
            for (int i = 0; i < len / 2; i++) {
                char c = buf[i];
                buf[i] = buf[len - 1 - i];
                buf[len - 1 - i] = c;
            }
        }

        for (int i = 0; i < len; i++) {
            /* Shift window left by one digit. */
            for (int j = 0; j < wlen; j++) win[j] = win[j + 1];
            if (wlen < L) wlen++;
            win[wlen - 1] = buf[i] - '0';

            pos++; /* increase digit position counter */

            if (wlen == L) { /* window is full */
                bool match = true;
                for (int k = 0; k < L && match; k++)
                    if (win[k] != pat[k]) match = false;

                if (match) {
                    seq_t idx = (seq_t)(pos - L + 1);
                    if (!foundFirst) {
                        first = idx;
                        foundFirst = true;
                    } else {
                        second = idx;
                        goto done;
                    }
                }
            }
        }
    }

done:
    return second - first;
}
