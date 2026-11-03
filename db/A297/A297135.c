/*
 * A297135  Numbers having a down-first zigzag pattern in base 6; see
 *          Comments.
 * https://oeis.org/A297135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the n-th integer whose base-6 representation has no equal
 *            consecutive digits and whose most significant digit is greater
 *            than the second most significant digit (i.e., a down‑first
 *            zigzag)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_down_first_zigzag(seq_t n)
{
    /* Extract base‑6 digits (least significant first). */
    seq_t tmp = n;
    int digits[200];          /* enough for any seq_t that fits in the build */
    int len = 0;

    if (tmp == 0)          /* single digit numbers are not allowed */
        return false;

    while (tmp > 0) {
        int d = (int)(tmp % 6);
        digits[len++] = d;
        tmp /= 6;
    }

    if (len < 2)           /* at least two digits required */
        return false;

    /* Most significant must be greater than the next digit. */
    int msb   = digits[len - 1];
    int next  = digits[len - 2];
    if (msb <= next)
        return false;

    /* All adjacent digits must differ (strict zigzag). */
    for (int i = 0; i < len - 1; ++i)
        if (digits[i] == digits[i + 1])
            return false;

    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n == 0; return 0 for safety */

    seq_t candidate = 1;   /* start searching from 1 upwards */
    idx_t count = 0;       /* number of qualifying numbers found so far */

    while (true) {
        if (is_down_first_zigzag(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;       /* move to the next integer */
    }
}
