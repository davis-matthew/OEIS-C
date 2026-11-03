/*
 * A281822  Least number k such that (k+n)^2 contains k as a substring.
 * https://oeis.org/A281822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = least k such that the decimal representation of (k+n)^2
 *            contains the decimal representation of k. Brute‑force search is
 *            used
 * Range:     u8 n <= 5, u16 n <= 495, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9867 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stddef.h>
#include <stdbool.h>

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

seq_t a(idx_t n)
{
    idx_t k = 1;
    /* buffers large enough for all expected values */
    static char subbuf[256];
    static char numbuf[4096];

    while (1)
    {
        /* compute (k + n)^2 as seq_t */
        seq_t sum = (seq_t)k + (seq_t)n;
        seq_t sq  = sum * sum;

        /* convert k to decimal string */
        size_t slen = sizeof(subbuf);
        seq_to_str((seq_t)k, subbuf, &slen);
        subbuf[slen] = '\0';

        /* convert (k+n)^2 to decimal string */
        size_t blen = sizeof(numbuf);
        seq_to_str(sq, numbuf, &blen);
        numbuf[blen] = '\0';

        /* check if k appears as a substring of (k+n)^2 */
        if (strstr(numbuf, subbuf) != NULL)
            return (seq_t)k;

        ++k;
    }
}
