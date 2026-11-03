/*
 * A261914  Largest palindrome < n (or 0 if n=0).
 * https://oeis.org/A261914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest palindrome < n (or 0 if n=0)
 * Range:     u8 n <= 261, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 83 terms through n = 8742 (2026-11-01)
 * Helpers:   is_palindrome_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_palindrome_base.h"

seq_t a(idx_t n) {
    /* n = 0 -> return 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* start from the largest candidate < n */
    seq_t cur = (seq_t)n - (seq_t)1;

    /* loop until a palindrome is found or we reach 0 */
    while (cur > (seq_t)0) {
        if (is_palindrome_base(cur, (idx_t)10)) {
            return cur;
        }
        cur = cur - (seq_t)1;
    }

    /* no palindrome found (happens only for n <= 2) */
    return (seq_t)0;
}
