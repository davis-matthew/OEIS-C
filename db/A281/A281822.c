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
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stddef.h>
#include "../utils/seq_to_str.h"

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
