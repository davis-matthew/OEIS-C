/*
 * A359982  Numbers whose digits are distinct nonprimes and are not a
 *          permutation of a smaller such number.
 * https://oeis.org/A359982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 61, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A359982: numbers whose digits are distinct nonprime digits
 * 0,1,4,6,8,9.  For each nonempty subset of these digits the
 * lexicographically smallest permutation that does not start with 0
 * (unless the subset consists of only 0) is taken.  The 63 resulting
 * numbers are sorted in ascending order and indexed from 1. */

/* Implementation note: all 63 terms are generated once and cached
 * in a static array.  The function is O(1) thereafter. */

#include <stdbool.h>

/* the set of nonprime digits */
static const int nonprime[6] = {0, 1, 4, 6, 8, 9};

seq_t a(idx_t n)
{
    static seq_t values[63];
    static bool initialized = false;

    if (!initialized) {
        /* generate the 63 minimal numbers */
        int k = 0;  /* number of values collected */
        for (int mask = 1; mask < 64; ++mask) {
            int digits[6];
            int cnt = 0;
            for (int i = 0; i < 6; ++i)
                if (mask & (1 << i))
                    digits[cnt++] = nonprime[i];

            /* sort the digits ascending */
            for (int i = 0; i < cnt - 1; ++i)
                for (int j = i + 1; j < cnt; ++j)
                    if (digits[i] > digits[j]) {
                        int t = digits[i];
                        digits[i] = digits[j];
                        digits[j] = t;
                    }

            /* build the minimal acceptable number */
            seq_t val = 0;
            if (cnt == 1) {
                val = digits[0];                     /* includes {0} */
            } else if (digits[0] == 0) {             /* ensure no leading 0 */
                int j = 1;
                while (j < cnt && digits[j] == 0) ++j;  /* find first non‑zero */
                int t = digits[0];
                digits[0] = digits[j];
                digits[j] = t;
                /* now compose */
                for (int i = 0; i < cnt; ++i)
                    val = val * 10 + digits[i];
            } else {
                for (int i = 0; i < cnt; ++i)
                    val = val * 10 + digits[i];
            }

            values[k++] = val;
        }

        /* sort the 63 numbers in ascending order */
        for (int i = 0; i < 63 - 1; ++i)
            for (int j = i + 1; j < 63; ++j)
                if (values[j] < values[i]) {
                    seq_t t = values[i];
                    values[i] = values[j];
                    values[j] = t;
                }

        initialized = true;
    }

    /* return the nth term (1‑based index); undefined for n<1 */
    if (n == 0 || n > 63)
        return 0;
    return values[n - 1];
}
