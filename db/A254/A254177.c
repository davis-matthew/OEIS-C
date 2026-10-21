/*
 * A254177  Decimal expansion of (1 u)c/h in m^-1.
 * https://oeis.org/A254177
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit_{n-14} of 75130066042530000, i.e., of
 *            7.513006604253 × 10^14, or 0 if out of range
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const char _digits[] = "75130066042530000";
static const size_t _digits_len = sizeof(_digits) - 1;  /* 17 digits */

seq_t a(idx_t n)
{
    if (n < 15) {                /* below the offset – undefined, return 0 */
        return (seq_t)0;
    }

    idx_t d = n - 14;              /* 1‑based index of the desired digit */

    if ((size_t)d > _digits_len) { /* beyond the stored digits → trailing zeros */
        return (seq_t)0;
    }

    return (seq_t)(_digits[d - 1] - '0');
}
