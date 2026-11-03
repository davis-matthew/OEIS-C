/*
 * A252043  a(n) is the concatenation of first n terms of A033307.
 * https://oeis.org/A252043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(C*10^n) where C = 0.123456789101112...
 *            (Champernowne constant)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 992 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Append the decimal digits of the integer k to the running result.
   The caller supplies the number of remaining digits to append in `rem`. */
static void append_int_to_res(idx_t k, seq_t *res, idx_t *rem)
{
    /* Maximum number of decimal digits of a uint64_t is 20;
       we allow a small extra buffer for safety. */
    uint8_t buf[22];
    int len = 0;

    if (k == 0) {
        buf[len++] = 0;
    } else {
        idx_t tmp = k;
        while (tmp > 0) {
            buf[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    /* Append digits in the correct order (most significant first). */
    for (int i = len - 1; i >= 0 && *rem > 0; --i) {
        *res = (*res) * 10 + (seq_t)buf[i];
        --(*rem);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined for n >= 1; return 0 for safety. */
        return (seq_t)0;
    }

    idx_t remaining = n;
    seq_t result = (seq_t)0;

    for (idx_t k = 1; remaining > 0; ++k) {
        append_int_to_res(k, &result, &remaining);
    }

    return result;
}
