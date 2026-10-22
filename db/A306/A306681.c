/*
 * A306681  a(1) = 1, for n >= 2 let the digits of a(n-1) be d_1, ..., d_i in
 *          base 10. Starting from d_1 do the following procedure: if d_r is
 *          divisible by 2, then d_r_new = d_r / 2, otherwise d_r_new = 3*d_r.
 *          a(n) = concatenation of d_r_new for r = 1 to i.
 * https://oeis.org/A306681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n >= 2 let the decimal digits of a(n-1) be
   d_1, …, d_k.  For each d_r, compute
   • if d_r even: d_r_new = d_r / 2
   • if d_r odd : d_r_new = 3 * d_r
   a(n) is the concatenation of the decimal strings of all d_r_new.
   The function below builds this sequence iteratively. */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;          /* undefined for n < 1, but return 0 */
    }

    /* start with a(1) = "1" */
    size_t cur_len = 1;
    char *cur = (char *)malloc(cur_len + 1);
    if (!cur) {
        return 0;          /* allocation failed */
    }
    cur[0] = '1';
    cur[1] = '\0';

    for (idx_t i = 2; i <= n; ++i) {
        /* Each digit can become at most two digits, so capacity is cur_len*2 */
        size_t new_cap = cur_len * 2;
        char *next = (char *)malloc(new_cap + 1);
        if (!next) {
            free(cur);
            return 0;
        }

        size_t new_len = 0;
        char *p = next;
        for (size_t j = 0; j < cur_len; ++j) {
            unsigned d = (unsigned)(cur[j] - '0');   /* digit 0-9 */

            unsigned new_d = (d & 1U) ? d * 3U : d >> 1U;  /* d/2 if even, 3*d if odd */

            if (new_d < 10U) {
                *p++ = '0' + new_d;
                ++new_len;
            } else {
                unsigned d1 = new_d / 10U;
                unsigned d2 = new_d % 10U;
                *p++ = '0' + d1;
                *p++ = '0' + d2;
                new_len += 2U;
            }
        }
        *p = '\0';

        free(cur);
        cur = next;
        cur_len = new_len;
    }

    seq_t result = seq_from_str(cur);
    free(cur);
    return result;
}
