/*
 * A359223  A "look-and-say" sequence. Describe the previous term by
 *          overstating the digit count by 1. a(1) = 1.
 * https://oeis.org/A359223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; a(n) = overstatement look‑and‑say of a(n‑1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   digit_len, get_digits, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: for n > 1 each run of identical decimal digits of length k   */
/* is replaced by the decimal representation of (k+1) followed by the digit */
/* itself.  This is the definition of OEIS A359223.                         */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/seq_from_str.h"
#include "../utils/digit_len.h"
#include "../utils/get_digits.h"

/* Return the nth term of the overstatement look‑and‑say sequence.   */
/* External linkage is required:                                            */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; n == 0 is out of range. */
    if (n == 0) return (seq_t)0;

    /* a(1) = 1. */
    seq_t cur = seq_from_str("1");
    if (n == 1) return cur;

    /* Iterate from 2 up to n, computing successive terms.          */
    for (idx_t step = 2; step <= n; ++step)
    {
        /* Length of current term in decimal digits. */
        size_t cur_len = digit_len(cur);

        /* Retrieve the decimal digits, least significant first.  */
        uint8_t *dig = (uint8_t *)malloc(cur_len);
        if (!dig) { /* malloc failed -> propagate zero value          */
            seq_t zero = (seq_t)0;
            /* We intentionally leak the previous value to keep the
               implementation simple; the caller will free() when it is
               done with the returned value. */
            return zero;
        }
        get_digits(cur, dig, &cur_len);
        /* cur_len is unchanged; it is the number of digits. */

        /* Allocate buffer for the next term.  It can be at most
           2*cur_len + 10 digits long.  */
        size_t out_cap = cur_len * 2 + 10;
        char *out = (char *)malloc(out_cap);
        if (!out) {
            free(dig);
            seq_t zero = (seq_t)0;
            return zero;
        }

        size_t pos = 0;   /* next write position in output buffer      */

        /* Process runs from the most significant digit to the least. */
        for (size_t i = 0; i < cur_len; /* i updated manually */)
        {
            /* Current digit (most significant first). */
            uint8_t d = dig[cur_len - 1 - i];

            /* Determine length of the run. */
            size_t run_len = 1;
            size_t j = i + 1;
            while (j < cur_len &&
                   dig[cur_len - 1 - j] == d)
            {
                ++run_len;
                ++j;
            }

            /* Overstated count = run_len + 1. */
            unsigned long long count = run_len + 1ULL;

            /* Write the decimal digits of 'count' into the output buffer. */
            char tmp[32];
            int tlen = 0;
            unsigned long long t = count;
            if (t == 0) {
                tmp[tlen++] = '0';
            } else {
                while (t > 0) {
                    tmp[tlen++] = (char)('0' + (t % 10));
                    t /= 10;
                }
            }
            for (int k = tlen - 1; k >= 0; --k) {
                out[pos++] = tmp[k];
            }

            /* Append the digit itself. */
            out[pos++] = '0' + d;

            /* Advance to the next run. */
            i = j;
        }

        /* Null‑terminate the string representation of the new term. */
        out[pos] = '\0';

        /* Convert the string back to a seq_t value. */
        seq_t next = seq_from_str(out);

        /* Clean up temporary buffers before the next iteration. */
        free(dig);
        free(out);

        /* Prepare for the next step. */
        cur = next;
    }

    /* Return the requested term.  The caller owns the returned value
       and its memory. */
    return cur;
}
