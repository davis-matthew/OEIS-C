/*
 * A357195  a(n) is the smallest palindrome of the form k*(2*n+k-1)/2 where k
 *          is a positive integer.
 * https://oeis.org/A357195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest palindrome of k*(2*n+k-1)/2, k>0
 * Range:     u8 n <= 11, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* Start with k = 1:   v_1 = n                                 */
    seq_t v = (seq_t)n;
    if (is_palindrome(v, (idx_t)10)) {
        return v;
    }

    /* Iterate k = 2, 3, ….
       v_k+1 = v_k + (k-1) + n   (derived from the closed form) */
    for (idx_t k = 2; ; ++k) {
        v += (seq_t)(k - 1 + n);
        if (is_palindrome(v, (idx_t)10)) {
            return v;
        }
    }
}
