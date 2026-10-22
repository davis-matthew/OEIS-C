/*
 * A397266  Lower (1,1) midsequence of (floor[n/2]^2) and (ceiling[n/3]^2);
 *          see Comments.
 * https://oeis.org/A397266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -a(n-1) + 2*a(n-3) + 3*a(n-4) + a(n-5) -a(n-6) - 3*a(n-7)
 *            - 2*a(n-8) + a(n-10) + a(n-11) with (a(0),…,a(11)) =
 *            (0,1,1,1,6,6,8,13,17,17,28,28)
 * Range:     u8 n <= 33, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t init[12] = {
    0ULL, 1ULL, 1ULL, 1ULL, 6ULL,
    6ULL, 8ULL, 13ULL, 17ULL, 17ULL,
    28ULL, 28ULL
};

seq_t a(idx_t n)
{
    /* Memoization buffer */
    static seq_t *buf = NULL;    /* points to the computed values */
    static idx_t buf_len = 0;     /* number of values currently stored   */

    /* initialise buffer with the first 12 terms if not yet done */
    if (buf_len == 0) {
        buf_len = 12;
        buf = (seq_t *)malloc((size_t)buf_len * sizeof(seq_t));
        if (buf == NULL) { exit(1); }
        memcpy(buf, init, (size_t)buf_len * sizeof(seq_t));
    }

    /* grow the buffer to include n */
    while (buf_len <= n) {
        idx_t i = buf_len;               /* index to be computed */
        seq_t val = 0;

        /* compute by recurrence; for i < 12 values are already set */
        if (i >= 12) {
            /* positive part: 2*a(i-3) + 3*a(i-4) + a(i-5) + a(i-10) + a(i-11) */
            seq_t sumPos = buf[i - 3] + buf[i - 3];
            sumPos += buf[i - 4] + buf[i - 4] + buf[i - 4];
            sumPos += buf[i - 5];
            sumPos += buf[i - 10];
            sumPos += buf[i - 11];

            /* negative part: a(i-1) + a(i-6) + 3*a(i-7) + 2*a(i-8) */
            seq_t sumNeg = buf[i - 1];
            sumNeg += buf[i - 6];
            sumNeg += buf[i - 7] + buf[i - 7] + buf[i - 7];
            sumNeg += buf[i - 8] + buf[i - 8];

            val = sumPos - sumNeg; /* guaranteed non‑negative */
        }

        /* append val to the buffer, extending it by 1 element */
        seq_t *new_buf = (seq_t *)realloc(buf,
                                          (size_t)(buf_len + 1) * sizeof(seq_t));
        if (new_buf == NULL) { exit(1); }
        buf = new_buf;
        buf[buf_len] = val;
        buf_len++;
    }

    return buf[n];
}
