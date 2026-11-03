/*
 * A360240  Weakly decreasing triples of positive integers sorted
 *          lexicographically and concatenated.
 * https://oeis.org/A360240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A331195(n-1)+1. The sequence is the digits (1‑based) of
 *            the concatenation of all weakly decreasing triples of positive
 *            integers sorted lexicographically. This implementation generates
 *            the triples in that order and extracts the requested digit
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Extract decimal digits of 'num' from most significant to least.
   'pos' is the current zero‑based position in the full digit stream.
   If the desired digit is found within 'num', it is stored in '*digit'
   and true is returned.  Otherwise false is returned. */
static bool digit_from_num(idx_t num, idx_t *pos, seq_t *digit)
{
    /* store digits in reverse order */
    int buf[50];
    int d = 0;
    idx_t tmp = num;

    while (tmp > 0) {
        buf[d++] = (int)(tmp % 10);
        tmp /= 10;
    }

    for (int i = d - 1; i >= 0; --i) {
        if (*pos == 0) {
            *digit = (seq_t)buf[i];
            return true;
        }
        --(*pos);
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, return 0 */

    idx_t pos = n - 1;              /* zero‑based index */
    seq_t digit = 0;

    for (idx_t x = 1; ; ++x) {
        for (idx_t y = 1; y <= x; ++y) {
            for (idx_t z = 1; z <= y; ++z) {
                if (digit_from_num(x, &pos, &digit)) return digit;
                if (digit_from_num(y, &pos, &digit)) return digit;
                if (digit_from_num(z, &pos, &digit)) return digit;
            }
        }
    }
}
