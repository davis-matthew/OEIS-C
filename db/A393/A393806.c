/*
 * A393806  Palindromes that have only one largest digit (strict maximum).
 * https://oeis.org/A393806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 251, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A393806:
   Palindromes of odd length whose largest digit occurs exactly once.
   For a palindrome of odd length the only digit that can occur once is the
   middle digit; thus a term is a palindrome where the middle digit is
   strictly larger than all other digits.  The sequence is produced
   in increasing numerical order. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)    /* offset is 1; n>=1 in valid calls */
        return 0;

    seq_t result = 0;
    idx_t count  = 0;

    /* ----- length 1 (digits 0..9) ----- */
    for (int d = 0; d <= 9 && count < n; ++d) {
        result = d;
        ++count;
    }

    /* ----- odd lengths >=3 ----- */
    for (int outer_len = 1; count < n; ++outer_len) {
        /* outer_len = number of outer digits (m-1 where m is left‑half length) */
        int *outer = (int *)malloc(sizeof(int) * outer_len);
        if (!outer) return 0;                     /* allocation failure */

        /* initialise outer digits to the smallest combination: 1 0 0 ... */
        for (int i = 0; i < outer_len; ++i)
            outer[i] = 0;
        outer[0] = 1;

        bool cont = true;
        while (cont) {
            /* find maximum digit among the outer digits */
            int max_d = outer[0];
            for (int i = 1; i < outer_len; ++i)
                if (outer[i] > max_d) max_d = outer[i];

            /* middle digit must be strictly greater than max_d */
            for (int mid = max_d + 1; mid <= 9 && count < n; ++mid) {
                seq_t val = 0;
                for (int i = 0; i < outer_len; ++i)
                    val = val * 10 + (seq_t)outer[i];
                val = val * 10 + (seq_t)mid;
                for (int i = outer_len - 1; i >= 0; --i)
                    val = val * 10 + (seq_t)outer[i];

                result = val;
                ++count;
            }

            /* increment the outer digit combination */
            int pos = outer_len - 1;
            while (pos >= 0) {
                ++outer[pos];
                if (pos == 0 && outer[pos] == 10) {
                    cont = false;   /* all combinations exhausted */
                    break;
                }
                if (outer[pos] == 10) {
                    outer[pos] = 0;
                    --pos;
                } else {
                    break;          /* successful increment */
                }
            }
        }
        free(outer);
    }

    return result;
}
