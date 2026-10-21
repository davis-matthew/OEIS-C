/*
 * A261914  Largest palindrome < n (or 0 if n=0).
 * https://oeis.org/A261914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest palindrome < n (or 0 if n=0)
 * Range:     u8 n <= 261, u16 n <= 8742, u32 n <= 8742, u64 n <= 8742,
 *            u128 n <= 8742, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_palindrome.h"

seq_t a(idx_t n) {
    /* n = 0 -> return 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* start from the largest candidate < n */
    seq_t cur = (seq_t)n - (seq_t)1;

    /* loop until a palindrome is found or we reach 0 */
    while (cur > (seq_t)0) {
        if (is_palindrome(cur, (idx_t)10)) {
            return cur;
        }
        cur = cur - (seq_t)1;
    }

    /* no palindrome found (happens only for n <= 2) */
    return (seq_t)0;
}
