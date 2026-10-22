/*
 * A333773  Replace 2's with (-1)'s in ternary representation of n and sum
 *          nonzero terms with alternating signs.
 * https://oeis.org/A333773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = sum of the non‑zero base‑3 digits of n after replacing 2
 *            by −1, with the terms added in decreasing order of significance
 *            and alternating signs starting with + on the most significant
 *            digit. See OEIS entry A333773
 * Range:     i8 n <= 242, i16 n <= 6561, i32 n <= 6561, i64 n <= 6561,
 *            i128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Collect base‑3 digits (least significant first). */
    uint8_t digits[64];
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = tmp % 3;
        tmp /= 3;
    }

    /* Precompute powers of 3 up to 3^(len-1). */
    seq_t pow[64];
    if (len > 0) {
        pow[0] = (seq_t)1;
        for (size_t i = 1; i < len; ++i)
            pow[i] = pow[i - 1] * (seq_t)3;
    }

    seq_t result = (seq_t)0;
    int sign = 1;                      /* +1 for the first term, then alternating */

    for (size_t idx = len; idx > 0; --idx) {
        uint8_t d = digits[idx - 1];
        if (d == 0)
            continue;                  /* skip zero digits */

        int coeff = (d == 1) ? 1 : -1; /* replace 2 with -1 */

        /* Multiply the coefficient, the power of 3, and the alternating sign. */
        seq_t term = pow[idx - 1] * (seq_t)(coeff * sign);
        result += term;
        sign = -sign;                  /* alternate for next non‑zero digit */
    }

    return result;
}
