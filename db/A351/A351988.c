/*
 * A351988  In the factorial base expansion of n, arrange digits in decreasing
 *          order.
 * https://oeis.org/A351988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by converting n to factorial base, sorting the
 *            digits in decreasing order, and re‑evaluating with the
 *            factorials in the same reversed order
 * Range:     u8 n <= 123, u16 n <= 5040, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* Maximum number of digits needed for a 64‑bit value is < 32. */
    seq_t digits[32];
    size_t k = 0;

    /* Factorial base conversion: digits[0] = coefficient of 1!,
       digits[1] = coefficient of 2!, … */
    idx_t m = n;
    for (size_t i = 1; m > 0; ++i) {
        digits[k++] = (seq_t)(m % (i + 1));
        m /= (i + 1);
    }

    /* Sort digits in descending order. */
    for (size_t i = 0; i < k; ++i) {
        for (size_t j = i + 1; j < k; ++j) {
            if (digits[i] < digits[j]) {
                seq_t t = digits[i];
                digits[i] = digits[j];
                digits[j] = t;
            }
        }
    }

    /* Recombine: largest digit goes to k!, next to (k‑1)!, …
       In other words, digits[i] (0‑based, already sorted descending)
       multiplies factorial (k‑i). */
    seq_t result = 0;
    seq_t fact   = 1;                /* fact = i! during the loop */
    for (size_t i = 1; i <= k; ++i) {
        fact *= (seq_t)i;            /* fact == i! */
        result += digits[k - i] * fact;
    }

    return result;
}
