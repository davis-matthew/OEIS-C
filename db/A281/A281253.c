/*
 * A281253  Alternately concatenate the decimal digits from front to back
 *          1...n such that n is always to the right.
 * https://oeis.org/A281253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   For n ≥ 1, a(n) is obtained by concatenating the integers 1…n *
 *            in an alternating order: * • If n is even: odds descending from
 *            n‑1 to 1, then evens ascending * from 2 to n‑2, finally n. * •
 *            If n is odd: evens descending from n‑1 to 2, then 1, then odds *
 *            ascending from 3 to n. In all cases n its...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    seq_t result = 0;

    if ((n & 1) == 0) {                     /* n even */
        /* odds descending from n-1 down to 1 */
        idx_t i = n - 1;
        while (i >= 1) {
            seq_t p = pow10((idx_t)digits(i));
            result = result * p + (seq_t)i;
            if (i == 1) break;
            i -= 2;
        }

        /* evens ascending from 2 to n-2 */
        for (idx_t i = 2; i <= n - 2; i += 2) {
            seq_t p = pow10((idx_t)digits(i));
            result = result * p + (seq_t)i;
        }

        /* finally n */
        seq_t p = pow10((idx_t)digits(n));
        result = result * p + (seq_t)n;
    }
    else {                                 /* n odd */
        /* evens descending from n-1 down to 2 */
        idx_t i = n - 1;
        while (i >= 2) {
            seq_t p = pow10((idx_t)digits(i));
            result = result * p + (seq_t)i;
            if (i == 2) break;
            i -= 2;
        }

        /* 1 */
        result = result * pow10((idx_t)digits(1)) + (seq_t)1;

        /* odds ascending from 3 to n */
        for (idx_t i = 3; i <= n; i += 2) {
            seq_t p = pow10((idx_t)digits(i));
            result = result * p + (seq_t)i;
        }
    }

    return result;
}
